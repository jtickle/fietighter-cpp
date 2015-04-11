#ifndef OGREAPPLICATION_H_DEFINED
#define OGREAPPLICATION_H_DEFINED

#include <Ogre.h>
//#include <OgreEventListeners.h>

class GUI;

class OgreApplication : public Ogre::FrameListener
{
public:
    OgreApplication(Ogre::String name = "OgreApplication");
    virtual ~OgreApplication();
    void addResourceLocationFrom(const Ogre::String& fileName);
    virtual void create(const Ogre::String& pluginsFileName,
        const Ogre::String& configFileName,
        const Ogre::String& logFileName,
        Ogre::SceneType sceneType);
    void run();

    virtual bool frameStarted(const Ogre::FrameEvent& evt);
    virtual bool frameEnded(const Ogre::FrameEvent& evt);
protected:
    virtual void init();
    void requestQuit();
    Ogre::StringVector      mResourcesFileNames;
    Ogre::String            mName;
    Ogre::Root*             mRoot;
    Ogre::RenderWindow*     mWindow;
    Ogre::SceneManager*     mSceneMgr;
    Ogre::Camera*           mCamera;
    Ogre::Viewport*         mViewport;
    GUI*                    mGUI;
    //Ogre::EventProcessor*   mEventProcessor;
    bool                    mQuitRequested;
};

#endif // OGREAPPLICATION_H_DEFINED
