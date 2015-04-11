#include "Ogre.h"
#include "PlayerManager.h"
#include "PlayerController.h"
#include "SmartController.h"
#include "HumanController.h"
#include "Player.h"
#include "GUI.h"
#include "FTLog.h"

PlayerManager::PlayerManager()
{
}

PlayerManager::~PlayerManager()
{
}

void PlayerManager::setSceneManager(Ogre::SceneManager* sceneMgr)
{
    mSceneMgr = sceneMgr;
}

void PlayerManager::setGameBoard(GameBoard* gameBoard)
{
    mGameBoard = gameBoard;
}

Player* PlayerManager::createPlayer(const Ogre::String& name, PlayerController *pc, bool showModel)
{
    Ogre::Vector3 initPos = Ogre::Vector3(
        (((Ogre::Real)rand() / RAND_MAX) * 10000) - 5000,
        (((Ogre::Real)rand() / RAND_MAX) * 10000) - 5000,
        (((Ogre::Real)rand() / RAND_MAX) * 10000) - 5000);
    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode(name + "PlayerNode", initPos);

    // TODO: Player Type - Factory?
    Player* player = new Player(node, mGameBoard, name, showModel);
    if(pc) {
        pc->setPlayer(player);
        pc->setPlayerManager(this);
    }
    mPlayerMap[name] = new ControllerPlayer(pc, player);
    return player;
}

Player* PlayerManager::createHumanPlayer(const Ogre::String& name, Ogre::RenderWindow *window, GUI* gui)
{
    // TODO: Handle Human Multiplayer, also this code should be somewhere else
    Ogre::Camera *camera = mSceneMgr->createCamera("Player1");
    camera->setNearClipDistance(NEAR_CLIP_DISTANCE);
    Ogre::Viewport *vp = window->addViewport(camera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));
    camera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

    HumanController* hc = new HumanController(gui);
    Player* player = createPlayer(name + "Human", hc, false);
    player->attachCamera(camera);
    return player;
}

Player* PlayerManager::getPlayer(const Ogre::String& name)
{
    return mPlayerMap[name]->player;
}

bool PlayerManager::playerExists(const Ogre::String& name)
{
    return mPlayerMap.find(name) != mPlayerMap.end();
}

void PlayerManager::queueDestroy(const Ogre::String& name)
{
    mDestroyQueue.push_back(name);
}

void PlayerManager::destroyPlayer(const Ogre::String& name)
{
    ControllerPlayer* cp = mPlayerMap[name];
    PlayerController *pc = cp->controller;
    Player* p            = cp->player;

    mPlayerMap.erase(name);

    // Notify Friends of Death
    PlayerMap::iterator i;
    for(i = mPlayerMap.begin(); i != mPlayerMap.end(); ++i) {
        ControllerPlayer* icp = (ControllerPlayer*)i->second;
        if(icp->controller) icp->controller->destroyNotify(p);
    }

    delete cp;
    delete pc;
    delete p;
}

void PlayerManager::doFrame(Ogre::Real fracSec, GUI* gui)
{
    if(!mDestroyQueue.empty()) {
        DestroyQueue::iterator idq;
        for(idq = mDestroyQueue.begin(); idq != mDestroyQueue.end(); ++idq) {
            if(playerExists((Ogre::String)(*idq))) {
                destroyPlayer((Ogre::String)*idq);
            }
        }
        mDestroyQueue.clear();
    }

    PlayerMap::iterator i;
    for(i = mPlayerMap.begin(); i != mPlayerMap.end(); ++i) {
        ControllerPlayer* cp = (ControllerPlayer*)i->second;
        if(cp->controller) {
            cp->controller->doFrame(fracSec);
        }
        cp->player->doFrame(fracSec);

        if(!cp->controller) continue;
        SmartController* c = dynamic_cast<SmartController*>(cp->controller);
        if(c) {
            gui->displayStats(cp->player->getName(), c->getStateString(),
                c->hasTarget() ? c->getTarget()->getName() : "NONE");
        }

        //gui->displayStats(cp->player->getName(), cp->player->getActualInertialSpeed(),
            //cp->player->getInertialPitch(), cp->player->getInertialYaw(),
            //cp->player->getInertialRoll());
    }
}

void PlayerManager::stopAllPlayers()
{
    PlayerMap::iterator i;
    for(i = mPlayerMap.begin(); i != mPlayerMap.end(); ++i) {
        ControllerPlayer* cp = (ControllerPlayer*)i->second;
        cp->player->setSpeed(0);
    }
}

void PlayerManager::goAllPlayers()
{
    PlayerMap::iterator i;
    for(i = mPlayerMap.begin(); i != mPlayerMap.end(); ++i) {
        ControllerPlayer* cp = (ControllerPlayer*)i->second;
        cp->player->setSpeed(1);
    }
}

void PlayerManager::visitAllPlayers(PlayerVisitor* p)
{
    PlayerMap::iterator i;
    for(i = mPlayerMap.begin(); i != mPlayerMap.end(); ++i) {
        ControllerPlayer* cp = (ControllerPlayer*)i->second;
        p->visit(cp->player, cp->controller);
    }
}
