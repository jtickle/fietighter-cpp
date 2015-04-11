#ifndef PLAYERFACTORY_H_INCLUDED
#define PLAYERFACTORY_H_INCLUDED

#include "Ogre.h"
#include "Player.h"
#include "HumanPlayer.h"

class PlayerFactory
{
public:
    virtual PlayerFactory();
    virtual ~PlayerFactory();

    static Player* getHumanPlayer(Ogre::SceneManager* sceneMgr, Ogre::RenderWindow *window);
};

#endif // PLAYERFACTORY_H_INCLUDED
