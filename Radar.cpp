#include "Radar.h"
#include "Player.h"
#include "PlayerController.h"
#include "GameBoard.h"
#include "PlayerManager.h"
#include "FTLog.h"
#include "Ogre.h"
#include <cmath>

#define TEX_WIDTH 256
#define TEX_HEIGHT 256

using namespace Ogre;

Radar::Radar(Player* player, GameBoard* gameboard) : mPlayer(player), mGameBoard(gameboard)
{
    Overlay* o = OverlayManager::getSingleton().create("RadarOverlay");
    mOverlay = OverlayManager::getSingleton().createOverlayElement("Panel", "RadarPanel");
    o->add2D((OverlayContainer*)mOverlay);
    mMaterial = MaterialManager::getSingleton().create("RadarMaterial", "General", true);

    // TODO: Base the size of the texture on the size of the screen.
    mTexture = TextureManager::getSingleton().createManual("RadarTexture", "General",
        TEX_TYPE_2D, TEX_WIDTH * 2, TEX_HEIGHT, MIP_DEFAULT, PF_X8R8G8B8, TU_STATIC|Ogre::TU_AUTOMIPMAP);

    Pass* pass = mMaterial->getTechnique(0)->getPass(0);
    TextureUnitState* tx = new TextureUnitState(pass);
    tx->setTextureName("RadarTexture");
    pass->addTextureUnitState(tx);

    mOverlay->setMetricsMode(GMM_PIXELS);
    mOverlay->setVerticalAlignment(GVA_TOP);
    mOverlay->setLeft(5);
    mOverlay->setTop(5);
    mOverlay->setWidth(TEX_WIDTH);
    mOverlay->setHeight(TEX_HEIGHT/2);
    mOverlay->setMaterialName("RadarMaterial");
    o->show();
}

Radar::~Radar()
{
}

void Radar::doFrame(Ogre::Real fracSec)
{
    drawScope();
}

void Radar::drawScope()
{
    HardwarePixelBufferSharedPtr buffer = mTexture->getBuffer();

    PixelBox destPb = buffer->lock(Image::Box(0, 0, TEX_WIDTH * 2, TEX_HEIGHT), HardwareBuffer::HBL_NORMAL);
    uint8* data = static_cast<uint8*>(destPb.data);

    const size_t pixelSize = PixelUtil::getNumElemBytes(destPb.format);
    const size_t rowPitchBytes = destPb.rowPitch * pixelSize;

    RadarPlayerVisitor* v = new RadarPlayerVisitor(mPlayer);
    mGameBoard->visitAllPlayers(v);

    ColourValue black = ColourValue(0.0,0.0,0.0,1.0);
    ColourValue white = ColourValue(1.0,0.0,0.0,1.0);

    // This is just awful.  Reset to black first.
    for(int z = 0; z <= 1; z++) {
        for(int x = 0; x < TEX_WIDTH * 2; x++) {
            for(int y = 0; y < TEX_HEIGHT; y++) {
                size_t offset = calcOffset(Vector3(x,y,z),pixelSize,rowPitchBytes);
                PixelUtil::packColour(black,destPb.format,&data[offset]);
            }
        }
    }

    // Draw Blips
    RadarPlayerVisitor::BlipList::iterator i;
    for(i = v->mBlips.begin(); i != v->mBlips.end(); ++i) {
        Vector3 pos = (Vector3)(*i);
        size_t offset = calcOffset(scopePosition(pos), pixelSize, rowPitchBytes);
        PixelUtil::packColour(white,destPb.format,&data[offset]);
    }

    delete v;

    buffer->unlock();
}

Vector3 Radar::scopePosition(Vector3 pos)
{
    Real pz = pos.z;
    Real z = 1;

    if(pos.z < 0) {
        z = 0;
        pz = -pz;
    }

    Real theta = acos(pz / sqrt(pow(pos.x,2) + pow(pos.y,2) + pow(pz,2)));
    Real phi   = (pos.x == 0 && pos.y == 0) ? M_PI : atan2(pos.y,pos.x);

    Real radius = (theta / M_PI) * TEX_WIDTH;

    Real x =  radius * cos(phi) + (TEX_WIDTH / 2);
    Real y = -radius * sin(phi) + (TEX_HEIGHT / 2);

    // The angle of a point on the scope is determined by its X and Y position
    // relative to mPlayer.  This angle is more or less atan(y/x), with special
    // considerations for zero.
    //Real angle = (pos.x == 0 && pos.y == 0) ? 0 : atan2(pos.y, pos.x);

    // The radius of a point on the scope is determined by its distance from
    // mPlayer.  Since the passed Vector3 is already relative to mPlayer, we
    // simply take its length to get the distance.  At its closest, the radius
    // should be TEX_WIDTH or TEX_HEIGHT.  As it gets farther away, it approaches 0.
    //Real radius = 100;   // pos.length();

    // This value is a simple hack to determine if the target Player is in front
    // or behind mPlayer, which affects which scope the blip will appear on.
    //Real behind = (pos.z > 0) ? 0 : 1;

    // We now build and return a new vector with X and Y calculated by the above
    // angle and radius, and Z is whether the blip is in front or behind mPlayer.
    //return Vector3((radius * sin(angle)) + (TEX_WIDTH/2),
    //               (radius * cos(angle)) + (TEX_HEIGHT/2),
    //                behind);

    return Vector3(x,y,z);
}

size_t Radar::calcOffset(Vector3 pos, const size_t& pixelSize, const size_t& rowPitchBytes)
{
    // Convention: ((z == 0) ? in front : behind) the ship.
    int x_offset = (int)pos.z * TEX_WIDTH;

    return (size_t)((int)pos.y * rowPitchBytes) + (((int)pos.x + x_offset) * pixelSize);
}

RadarPlayerVisitor::RadarPlayerVisitor(Player* player) :
    mPlayer(player)
{
}

RadarPlayerVisitor::~RadarPlayerVisitor()
{
}

void RadarPlayerVisitor::visit(Player* p, PlayerController* pc)
{
    if(p == mPlayer) return;    // Don't put the human on the radar

    mBlips.push_back(mPlayer->getRelativePositionOf(p));
}
