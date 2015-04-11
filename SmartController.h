#ifndef SMARTCONTROLLER_H_DEFINED
#define SMARTCONTROLLER_H_DEFINED

#include "PlayerController.h"
#include "PlayerManager.h"

namespace Ogre {
    class Timer;
    class Vector3;
}

class FindClosestVisitor : public PlayerVisitor
{
public:
    FindClosestVisitor(Player* player);
    virtual ~FindClosestVisitor();

    void visit(Player* p, PlayerController* pc);

    Player* closest;

protected:
    Player* mPlayer;
    Ogre::Real minRange;
};

class SmartController : public PlayerController
{
public:
    SmartController();
    virtual ~SmartController();

    void doFrame(Ogre::Real fracSec);
    void destroyNotify(Player* p);

    typedef enum {
        PS_HUNTING,
        PS_TRACKING,
        PS_ATTACKING,
        PS_EVADING
    } PlayerState;

    Ogre::String getStateString();
    Player* getTarget();
    bool hasTarget();

protected:
    void doHunt(Ogre::Real fracSec);
    void doTrack(Ogre::Real fracSec);
    void doAttack(Ogre::Real fracSec);
    void doEvade(Ogre::Real fracSec);

    void moveRandom(Ogre::Real fracSec);

    Player* mTarget;
    PlayerState mState;
    Ogre::Timer* mTimer;
};

#endif // SMARTCONTROLLER_H_DEFINED

