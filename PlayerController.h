#ifndef PLAYERCONTROLLER_H_DEFINED
#define PLAYERCONTROLLER_H_DEFINED

#include "Ogre.h"

class Player;
class PlayerManager;

class PlayerController
{
public:
    PlayerController();
    virtual void setPlayerManager(PlayerManager* pm);
    virtual void doFrame(Ogre::Real fracSec) = 0;
    virtual void setPlayer(Player* p);
    virtual void forgetPlayer();
    virtual void destroyNotify(Player* p);

protected:
    Player* mPlayer;
    PlayerManager* mPlayerMgr;
};

#endif // PLAYERCONTROLLER_H_DEFINED
