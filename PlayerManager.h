#ifndef PLAYERMANAGER_H_INCLUDED
#define PLAYERMANAGER_H_INCLUDED

#include "Ogre.h"
#include <list>

#define NEAR_CLIP_DISTANCE 5

class Player;
class PlayerController;
class GameBoard;
class GUI;

// TODO: Support FlightGroups and Teams

class PlayerVisitor
{
public:
    virtual void visit(Player* p, PlayerController* pc) = 0;
};

class PlayerManager
{
public:
    PlayerManager();
    virtual ~PlayerManager();

    // These have to be used before a Player can be created.
    virtual void setSceneManager(Ogre::SceneManager* sceneMgr);
    virtual void setGameBoard(GameBoard* gameBoard);

    virtual Player* createPlayer(const Ogre::String& name, PlayerController* pc, bool showModel = true);
    virtual Player* createHumanPlayer(const Ogre::String& name, Ogre::RenderWindow *window, GUI* mGUI);
    virtual Player* getPlayer(const Ogre::String& name);
    virtual bool playerExists(const Ogre::String& name);
    virtual void queueDestroy(const Ogre::String& name);

    virtual void stopAllPlayers();
    virtual void goAllPlayers();

    virtual void doFrame(Ogre::Real fracSec, GUI* gui);

    void visitAllPlayers(PlayerVisitor* pv);
public:
    typedef struct ControllerPlayer {
        PlayerController* controller;
        Player*           player;
        ControllerPlayer() { };
        ControllerPlayer(PlayerController* pc, Player* p) : controller(pc), player(p) { };
    } ControllerPlayer;

    typedef HashMap<Ogre::String, ControllerPlayer*> PlayerMap;
    // typedef HashMap<Ogre::String, PlayerMap> FlightGroup;
    // typedef HashMap<Ogre::String, FlightGroup> Team;

    typedef std::list<Ogre::String> DestroyQueue;

protected:
    virtual void destroyPlayer(const Ogre::String& name);

    PlayerMap mPlayerMap;
    Ogre::SceneManager* mSceneMgr;
    GameBoard* mGameBoard;
    DestroyQueue mDestroyQueue;
};


#endif // PLAYERMANAGER_H_INCLUDED
