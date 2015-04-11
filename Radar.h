#ifndef RADAR_H_DEFINED
#define RADAR_H_DEFINED

#include "Ogre.h"
#include "PlayerManager.h"
#include <list>

class Player;
class PlayerController;
class GameBoard;

class RadarPlayerVisitor : public PlayerVisitor
{
public:
    RadarPlayerVisitor(Player* player);
    virtual ~RadarPlayerVisitor();

    void visit(Player* p, PlayerController* pc);

    typedef std::list<Ogre::Vector3> BlipList;

    BlipList mBlips;

protected:
    Player* mPlayer;
};

class Radar
{
public:
    Radar(Player* player, GameBoard* gameboard);
    virtual ~Radar();

    virtual void doFrame(Ogre::Real fracSec);

protected:
    virtual void drawScope();
    size_t calcOffset(Ogre::Vector3 pos, const size_t& pixelSize, const size_t& rowPitchBytes);
    Ogre::Vector3 scopePosition(Ogre::Vector3 pos);
    Ogre::OverlayElement* mOverlay;
    Ogre::MaterialPtr     mMaterial;
    Ogre::TexturePtr      mTexture;
    Player*               mPlayer;
    GameBoard*            mGameBoard;
};

#endif // RADAR_H_DEFINED
