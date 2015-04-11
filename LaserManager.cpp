#include "LaserManager.h"
#include "Player.h"
#include "Ogre.h"

using namespace Ogre;

// TODO: This should all probably be rolled up in the Player system.

LaserManager::LaserManager()
{
}

LaserManager::~LaserManager()
{
}

void LaserManager::setSceneManager(Ogre::SceneManager* sceneMgr)
{
    mSceneMgr = sceneMgr;
}

void LaserManager::createLaserBeam(Player* p)
{
    SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode(p->getPosition(), p->getOrientation());
    ManualObject *manual = mSceneMgr->createManualObject(NULL);

    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_LIST);
    manual->colour(0.0,1.0,0.0,1.0);
    manual->position(0.0,0.0,0.0);
    manual->position(0.0,0.0,-10000.0);
    manual->end();
    node->attachObject(manual);

    mLaserList.push_back(node);
}

bool RemoveIt(SceneNode* node)
{
    return ((ManualObject*)node->getAttachedObject(0))->getSection(0)->getMaterial()->getTechnique(0)->getPass(0)->getAmbient().a <= 0;
}

void LaserManager::doFrame(Ogre::Real fracSec)
{
    LaserList::iterator i;
    for(i = mLaserList.begin(); i != mLaserList.end(); i++) {
        SceneNode *node = (*i);
        ManualObject *manual = (ManualObject*)node->getAttachedObject(0);
        Ogre::Real alpha = manual->getSection(0)->getMaterial()->getTechnique(0)->getPass(0)->getAmbient().a;

        // Fade it just a little bit
        alpha -= fracSec * FADE_PER_SEC;

        manual->clear();
        manual->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_LIST);
        manual->colour(ColourValue::Green);
        manual->position(0.0,0.0,0.0);
        manual->position(0.0,0.0,-10000.0);
        manual->end();
    }

    // What sort of black magic is this??
    mLaserList.erase(remove_if(mLaserList.begin(), mLaserList.end(), RemoveIt), mLaserList.end());
}
