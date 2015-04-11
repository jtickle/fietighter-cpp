#include "Ogre.h"
#include "OgreErrorDialog.h"
#include "OgrePlatform.h"

#include "OgreApplication.h"

#include "Player.h"
#include "GameBoard.h"
#include "input.h"
#include "GUI.h"
#include "Radar.h"

#include "NascarController.h"
#include "RandomController.h"
#include "SmartController.h"
#include "StationaryController.h"

#include "OgreWindowEventUtilities.h"

#include "FTLog.h"

#include <stdlib.h>
#include <time.h>

class FieTighter : public OgreApplication
{
public:
    FieTighter();
protected:
    virtual void init();
    virtual bool frameStarted(const Ogre::FrameEvent &evt);
    virtual void doShipMesh();
    InputHandler* mInputHandler;
    GameBoard* mGameBoard;
    Player* mHuman;
    Radar* mRadar;
};

using namespace Ogre;
using namespace std;

FieTighter::FieTighter() : OgreApplication("FieTighter")
{
}

void FieTighter::init()
{
    doShipMesh();

    mSceneMgr->setSkyBox(true, "SkyBox/AsteroidSkyBox");

    mGameBoard = new GameBoard(mSceneMgr, mGUI);
    mInputHandler = new InputHandler(mWindow, mGameBoard, mGUI);

    //StationaryController *rc = new StationaryController();
    //Player *d = mGameBoard->createPlayer("Dummy1", rc);

    SmartController *rc = new SmartController();
    mGameBoard->createPlayer("Bart", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Homer", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Lisa", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Marge", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Maggie", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Otto", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Comic Book Guy", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Mr. Burns", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Bumblebee Man", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Moe", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Smithers", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Barney", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Flanders", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Rod", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Todd", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Maude", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Santa's Little Helper", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Snowball 1-7", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Spiderpig", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Karl", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Lenny", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Duffman", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Prof. Frink", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Milhouse", rc);

    rc = new SmartController();
    mGameBoard->createPlayer("Principal Skinner", rc);

    mHuman = mGameBoard->createHumanPlayer("Player1", mWindow, mGUI);
    mInputHandler->setPlayer(mHuman);

    // Radar
    mRadar = new Radar(mHuman, mGameBoard);

    /*Ogre::Overlay* mPanelOverlay;
    mPanelOverlay = Ogre::OverlayManager::getSingleton().getByName("FieTighter/Debug");
    mPanelOverlay->show();*/
}

void FieTighter::doShipMesh()
{
    // Mesh Data
    static const size_t nbVertices        = 9;
    static const size_t nbFaces           = 6;
    static const size_t nbVerticesPerFace = 3;
    static const Vector3 positions[nbVertices] = {
        Vector3( -10.0f,  10.0f,  20.0f),
        Vector3( -10.0f, -10.0f,  20.0f),
        Vector3(  10.0f,  10.0f,  20.0f),
        Vector3(  10.0f, -10.0f,  20.0f),
        Vector3(  0.0f,  0.0f, -20.0f),
        Vector3( -10.0f,  10.0f,  20.0f),
        Vector3(  10.0f,  10.0f,  20.0f),
        Vector3(  10.0f, -10.0f,  20.0f),
        Vector3( -10.0f, -10.0f,  20.0f)
    };
    //static const Vector2 tcoordinates[nbVertices] = {
        // TODO
    //}
    static const unsigned short faces[nbFaces][nbVerticesPerFace] = {
        {0,1,3},    // Back
        {3,2,0},    // Back
        {5,6,4},    // Top
        {6,7,4},    // Right
        {7,8,4},    // Bottom
        {8,5,4}     // Left
    };

    // Create a mesh
    MeshPtr mesh = MeshManager::getSingleton().createManual("ship.mesh", "Custom");
    SubMesh* submesh = mesh->createSubMesh();
    submesh->useSharedVertices = false;
    submesh->vertexData = new VertexData();
    submesh->vertexData->vertexStart = 0;
    submesh->vertexData->vertexCount = nbVertices;

    VertexDeclaration* declaration = submesh->vertexData->vertexDeclaration;
    static const unsigned short source = 0;
    size_t offset = 0;
    offset += declaration->addElement(source, offset, VET_FLOAT3, VES_POSITION).getSize();
    offset += declaration->addElement(source, offset, VET_FLOAT3, VES_NORMAL).getSize();
    //offset += declaration->addElement(source, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES).getSize();

    HardwareVertexBufferSharedPtr vbuffer =
        HardwareBufferManager::getSingleton().createVertexBuffer(declaration->getVertexSize(source),
            submesh->vertexData->vertexCount,
            HardwareBuffer::HBU_STATIC_WRITE_ONLY);

    // Fill the buffer
    AxisAlignedBox aabox;
    float* vdata = static_cast<float*>(vbuffer->lock(HardwareBuffer::HBL_DISCARD));
    for(size_t i = 0; i < nbVertices; ++i)
    {
        // Position
        Vector3 position = positions[i];
        *vdata++ = position.x;
        *vdata++ = position.y;
        *vdata++ = position.z;
        aabox.merge(position);
        // Normal
        Vector3 normal = position.normalisedCopy();
        *vdata++ = normal.x;
        *vdata++ = normal.y;
        *vdata++ = normal.z;
    }
    vbuffer->unlock();
    submesh->vertexData->vertexBufferBinding->setBinding(source, vbuffer);

    // Index Data
    submesh->indexData->indexStart = 0;
    submesh->indexData->indexCount = nbFaces*nbVerticesPerFace;
    submesh->indexData->indexBuffer =
        HardwareBufferManager::getSingleton().createIndexBuffer(HardwareIndexBuffer::IT_16BIT,
            submesh->indexData->indexCount,
            HardwareBuffer::HBU_STATIC_WRITE_ONLY);
    uint16* idata = static_cast<uint16*>(submesh->indexData->indexBuffer->lock(HardwareBuffer::HBL_DISCARD));
    for(size_t i = 0; i < nbFaces; ++i)
    {
        for(size_t j = 0; j < nbVerticesPerFace; ++j)
        {
            *idata++ = faces[i][j];
        }
    }
    submesh->indexData->indexBuffer->unlock();

    // We must indicate the bounding box
    mesh->_setBounds(aabox);
    mesh->_setBoundingSphereRadius((aabox.getMaximum()-aabox.getMinimum()).length()/2.0);
    //submesh->setMaterialName("Tutorial4/Dice");
    mesh->load();
}

bool FieTighter::frameStarted(const Ogre::FrameEvent &evt)
{
    Ogre::WindowEventUtilities::messagePump();
    mInputHandler->capture();
    if(mInputHandler->requestedTerminate()) return false;
    mGameBoard->doFrame(evt.timeSinceLastFrame);
    mRadar->doFrame(evt.timeSinceLastFrame);
    mGUI->displaySpeed(mHuman->getDisplaySpeed());
    return true;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else // OGRE_PLATFORM != OGRE_PLATFORM_WIN32
int main(int argc, char *argv[])
#endif // OGRE_PLATFORM == OGRE_PLATFORM_WIN32
{
    srand(time(NULL));

    FieTighter* app = new FieTighter();
    try {
        app->addResourceLocationFrom("resources2.cfg");
        app->create("plugins.cfg", "ogre.cfg", "ogre.log", Ogre::ST_GENERIC);
        app->run();
    } catch(Ogre::Exception& e) {
        MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
        return 1;
    }

    FTLog << "Exiting Now\n";
    FTLog.close();

    return 0;
}
