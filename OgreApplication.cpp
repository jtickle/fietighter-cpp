#include "ogreapplication.h"
#include "CEGUI.h"
#include "GUI.h"
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUIResourceProvider.h"
using namespace Ogre;
using namespace std;

namespace
{
    void parseResourceLocationFrom(const String& resourcesFileName)
    {
        // Load resource paths from config file
        ConfigFile cf;
        cf.load(resourcesFileName);

        // Go through all sections & settings in the file
        ConfigFile::SectionIterator seci = cf.getSectionIterator();
        while(seci.hasMoreElements())
        {
            String secName = seci.peekNextKey();
            ConfigFile::SettingsMultiMap* settings = seci.getNext();
            for(ConfigFile::SettingsMultiMap::iterator i = settings->begin();
                    i != settings->end(); ++i) {
                String typeName = i->first;
                String archName = i->second;
                ResourceGroupManager::getSingleton().addResourceLocation(
                    archName, typeName, secName);
            }
        }
    }
}

OgreApplication::OgreApplication(Ogre::String name) : mName(name),
        mRoot(NULL), mWindow(NULL),// mEventProcessor(NULL),
        mQuitRequested(false)
{
}

OgreApplication::~OgreApplication()
{
//    if(mEventProcessor) delete mEventProcessor;
    if(mRoot)           delete mRoot;
}

void OgreApplication::addResourceLocationFrom(const String& fileName)
{
    mResourcesFileNames.push_back(fileName);
}

void OgreApplication::create(const String& pluginsFileName,
        const String& configFileName,
        const String& logFileName,
        SceneType sceneType)
{
    mRoot = new Root(pluginsFileName, configFileName, logFileName);
    if(!mRoot->restoreConfig())
    {
        if(!mRoot->showConfigDialog())
        {
            throw Exception(0, "user quitted config dialog", "OgreApplication::create()", "Type?", __FILE__, __LINE__);
        }
    }

    mWindow = mRoot->initialise(true, mName);
    mWindow->setFullscreen(false, 1024, 768);

    for_each(mResourcesFileNames.begin(), mResourcesFileNames.end(), parseResourceLocationFrom);
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    mSceneMgr = mRoot->createSceneManager(sceneType, "sceneManager");

    //CEGUI::OgreRenderer& renderer CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::OgreCEGUIRenderer* renderer = new CEGUI::OgreCEGUIRenderer(mWindow,
        Ogre::RENDER_QUEUE_OVERLAY, false, 3000, mSceneMgr);
    mGUI = new GUI(renderer);

    init();

    mRoot->addFrameListener(this);
}

void OgreApplication::run()
{
    mRoot->startRendering();
}

bool OgreApplication::frameStarted(const FrameEvent& evt)
{
    return true;
}

bool OgreApplication::frameEnded(const FrameEvent& evt)
{
    return !mQuitRequested;
}

void OgreApplication::init()
{
}

void OgreApplication::requestQuit()
{
    mQuitRequested = true;
}
