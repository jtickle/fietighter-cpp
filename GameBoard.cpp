#include "GameBoard.h"
#include "Player.h"
#include "PlayerManager.h"
#include "PlayerController.h"
#include "LaserManager.h"
#include "input.h"
#include "GUI.h"
#include "FTLog.h"

GameBoard::GameBoard(Ogre::SceneManager* sceneMgr, GUI* gui) : mSceneMgr(sceneMgr), mGUI(gui)
{
    mPlayerMgr = new PlayerManager();
    mPlayerMgr->setSceneManager(sceneMgr);
    mPlayerMgr->setGameBoard(this);
    //mLaserMgr = new LaserManager();
    //mLaserMgr->setSceneManager(sceneMgr);
}

GameBoard::~GameBoard()
{
    delete mPlayerMgr;
    //delete mLaserMgr;
}

Player* GameBoard::createPlayer(const Ogre::String& name, PlayerController* pc)
{
    return mPlayerMgr->createPlayer(name, pc);
}

Player* GameBoard::createHumanPlayer(const Ogre::String& name, Ogre::RenderWindow *window, GUI* gui)
{
    return mPlayerMgr->createHumanPlayer(name, window, gui);
}

void GameBoard::doFrame(Ogre::Real fracSec)
{
    mPlayerMgr->doFrame(fracSec, mGUI);
    //mLaserMgr->doFrame(fracSec);
}

void GameBoard::stopAllPlayers()
{
    mPlayerMgr->stopAllPlayers();
}

void GameBoard::goAllPlayers()
{
    mPlayerMgr->goAllPlayers();
}

void GameBoard::visitAllPlayers(PlayerVisitor* pv)
{
    mPlayerMgr->visitAllPlayers(pv);
}

void GameBoard::blowUp(Player* killer, Player* target)
{
    mPlayerMgr->queueDestroy(target->getName());
    Ogre::String verb = " wtf ";
    switch(rand() % 5) {
        case 0: verb = " blew up "; break;
        case 1: verb = " destroyed "; break;
        case 2: verb = " obliterated "; break;
        case 3: verb = " nailed "; break;
        case 4: verb = " ruined "; break;
        case 5: verb = " pwnt "; break;
        case 6: verb = " fired on "; break;
        case 7: verb = " shafted "; break;
        case 8: verb = " blasted "; break;
        case 9: verb = " ate "; break;
    }
    //mLaserMgr->createLaserBeam(killer);
    mGUI->displayKill(killer->getName() + verb + target->getName());
}
