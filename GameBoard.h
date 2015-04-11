#ifndef GAMEBOARD_H_INCLUDED
#define GAMEBOARD_H_INCLUDED

#include "Ogre.h"

#include <vector>

// Today I learned about Forward Declarations.
class Player;
class InputHandler;
class PlayerManager;
class GUI;
class PlayerController;
class PlayerVisitor;
class LaserManager;

class GameBoard
{
public:
    GameBoard(Ogre::SceneManager *sceneMgr, GUI* gui);
    virtual ~GameBoard();

    void doFrame(Ogre::Real fracSec);

    Player* createPlayer(const Ogre::String& name, PlayerController* pc);
    Player* createHumanPlayer(const Ogre::String& name, Ogre::RenderWindow *window, GUI* gui);

    void stopAllPlayers();
    void goAllPlayers();

    Ogre::SceneManager* getSceneManager() { return mSceneMgr; };

    void visitAllPlayers(PlayerVisitor* pv);

    virtual void blowUp(Player* killer, Player* target);
protected:
    Ogre::SceneManager* mSceneMgr;
    //GameState* mGameState;
    PlayerManager*      mPlayerMgr;
    LaserManager*       mLaserMgr;
    GUI*                mGUI;
};

#endif // GAMEBOARD_H_INCLUDED
