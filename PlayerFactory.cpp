#include "PlayerFactory.h"

static Player* PlayerFactory::getHumanPlayer(Ogre::SceneManager* sceneMgr, Ogre::RenderWindow *window)
{
    // TODO: Handle Human Multiplayer
    Ogre::Camera *camera = sceneMgr->createCamera("Player1");
    camera->setNearClipDistance(NEAR_CLIP_DISTANCE);
    Ogre::Viewport *vp = window->addViewport(camera);
    vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

    Ogre::SceneNode *node = sceneMgr->getRootSceneNode()->createChildSceneNode("humanPlayerNode", Ogre::Vector3(0, 0, 0));
    node->attachCamera(camera);

    // TODO: this, right
    camera->setAspectRatio((Ogre::Real)1.333333);

    HumanPlayer* player = new HumanPlayer(node);
    return player;
}
