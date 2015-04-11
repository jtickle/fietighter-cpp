#ifndef LASERMANAGER_H_INCLUDED
#define LASERMANAGER_H_INCLUDED

#include "Ogre.h"
#include <list>

#define FADE_PER_SEC 4.0;

class Player;

class LaserManager
{
public:
    LaserManager();
    virtual ~LaserManager();

    virtual void setSceneManager(Ogre::SceneManager* sceneMgr);
    virtual void createLaserBeam(Player* p);
    virtual void doFrame(Ogre::Real fracSec);

    typedef std::list<Ogre::SceneNode*> LaserList;

protected:
    Ogre::SceneManager* mSceneMgr;
    LaserList           mLaserList;

};

#endif // LASERMANAGER_H_INCLUDED
