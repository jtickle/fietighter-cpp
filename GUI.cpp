#include "GUI.h"
#include "CEGUI.h"
#include "OIS/OISInputManager.h"
#include "OIS/OISMouse.h"
#include "OIS/OISKeyboard.h"
#include "OIS/OISJoyStick.h"
#include "Ogre.h"
#include "OgreStringConverter.h"

using namespace CEGUI;
using Ogre::StringConverter;

GUI::GUI(Renderer* renderer) : mRenderer(renderer), mStatsPos(0)
{
    mSystem = new System(mRenderer);
    CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);

    SchemeManager::getSingleton().loadScheme((utf8*)"TaharezLook.scheme",(utf8*)"General");
    //mSystem->setDefaultMouseCursor((utf8*)"TaharezLook",(utf8*)"MouseArrow");
    //MouseCursor::getSingleton().setImage(System::getSingleton().getDefaultMouseCursor());

    createInGameLayout();
}

void GUI::createInGameLayout()
{
    WindowManager& wmgr = WindowManager::getSingleton();

    mInGameWindow = wmgr.createWindow("DefaultWindow", "InGame");
    System::getSingleton().setGUISheet(mInGameWindow);

    FrameWindow* fWnd = (FrameWindow*)wmgr.createWindow("TaharezLook/FrameWindow", "setSpeed");
    mInGameWindow->addChildWindow(fWnd);
    fWnd->setPosition(UVector2(UDim(1.0f, -130), UDim(0.5f, -20)));
    fWnd->setSize(UVector2(UDim(0, 125), UDim(0, 10)));
    fWnd->setText("Speed");

    mSpeedDisplay = wmgr.createWindow("TaharezLook/StaticText", "setSpeedDisplay");
    fWnd->addChildWindow(mSpeedDisplay);
    mSpeedDisplay->setPosition(UVector2(UDim(0.0f, 5), UDim(0.0f, 5)));
    mSpeedDisplay->setText("0");

    fWnd = (FrameWindow*) wmgr.createWindow("TaharezLook/FrameWindow", "LogWindow");
    mInGameWindow->addChildWindow(fWnd);
    fWnd->setPosition(UVector2(UDim(0.0f, 5), UDim(1.0f, -305)));
    fWnd->setSize(UVector2(UDim(0, 400), UDim(0, 300)));
    fWnd->setText("Log");
    mLogWindow = wmgr.createWindow("TaharezLook/StaticText", "LogElement");
    fWnd->addChildWindow(mLogWindow);
    mLogWindow->setPosition(UVector2(UDim(0.0f, 0), UDim(0.0f, 0)));
    mLogWindow->setSize(UVector2(UDim(1.0f, 0), UDim(1.0f, 0)));
    mLogWindow->setText("Logging Started\n");
}

MouseButton GUI::convertButton(OIS::MouseButtonID buttonID)
{
    switch(buttonID)
    {
        default:
        case OIS::MB_Left:
            return CEGUI::LeftButton;

        case OIS::MB_Middle:
            return CEGUI::MiddleButton;

        case OIS::MB_Right:
            return CEGUI::RightButton;
    }
}

void GUI::mousePressed(OIS::MouseButtonID id)
{
    System::getSingleton().injectMouseButtonDown(convertButton(id));
}

void GUI::mouseReleased(OIS::MouseButtonID id)
{
    System::getSingleton().injectMouseButtonUp(convertButton(id));
}

void GUI::mouseMoved(Ogre::Real x, Ogre::Real y)
{
    System::getSingleton().injectMouseMove(x, y);
}

void GUI::displaySpeed(Ogre::String speed)
{
    mSpeedDisplay->setText(speed);
}

void GUI::createStatsWindow(const Ogre::String& name, const Ogre::String& title)
{
    WindowManager& wmgr = WindowManager::getSingleton();

    FrameWindow* stats = (FrameWindow*)wmgr.createWindow("TaharezLook/FrameWindow", name);
    mInGameWindow->addChildWindow(stats);

    mStatsPos -= 75;
    stats->setPosition(UVector2(UDim(0.0f, 5), UDim(1.0f, mStatsPos)));
    stats->setSize(UVector2(UDim(0,250), UDim(0,10)));
    stats->setText(name);

    Window* element = wmgr.createWindow("TaharezLook/StaticText", name + "State");
    stats->addChildWindow(element);
    element->setPosition(UVector2(UDim(0.0f, 5), UDim(0.0f, 5)));
    element->setText("0");

    element = wmgr.createWindow("TaharezLook/StaticText", name + "Target");
    stats->addChildWindow(element);
    element->setPosition(UVector2(UDim(0.0f, 120), UDim(0.0f, 5)));
    element->setText("0");
}

void GUI::displayStats(Ogre::String name, Ogre::String state, Ogre::String target)
{
    /*WindowManager& wmgr = WindowManager::getSingleton();
    Ogre::String winName = name + "Debug";

    if(!wmgr.isWindowPresent(winName)) {
        createStatsWindow(winName, name);
    }

    Window* element = 0;

    element = wmgr.getWindow(winName + "State");
    element->setText(state);

    element = wmgr.getWindow(winName + "Target");
    element->setText(target);*/
}

void GUI::displayKill(Ogre::String message)
{
    mLogWindow->setText(mLogWindow->getText() + message + "\n");
}

void GUI::humanStats(Ogre::String smartass, Ogre::String name, Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
    WindowManager& wmgr = WindowManager::getSingleton();
    Ogre::String winName = "HumanStats";

    if(!wmgr.isWindowPresent(winName)) {
        FrameWindow* stats = (FrameWindow*)wmgr.createWindow("TaharezLook/FrameWindow", winName);
        mInGameWindow->addChildWindow(stats);

        stats->setPosition(UVector2(UDim(1.0f, -300), UDim(1.0f, -75)));
        stats->setSize(UVector2(UDim(0,300), UDim(0,10)));
        stats->setText("Nothing Yet");

        Window* element = wmgr.createWindow("TaharezLook/StaticText", winName + "Smartass");
        stats->addChildWindow(element);
        element->setPosition(UVector2(UDim(0.0f, 5), UDim(0.0f, 5)));
        element->setText("0");

        element = wmgr.createWindow("TaharezLook/StaticText", winName + "X");
        stats->addChildWindow(element);
        element->setPosition(UVector2(UDim(0.0f, 150), UDim(0.0f, 5)));
        element->setText("0");

        element = wmgr.createWindow("TaharezLook/StaticText", winName + "Y");
        stats->addChildWindow(element);
        element->setPosition(UVector2(UDim(0.0f, 200), UDim(0.0f, 5)));
        element->setText("0");

        element = wmgr.createWindow("TaharezLook/StaticText", winName + "Z");
        stats->addChildWindow(element);
        element->setPosition(UVector2(UDim(0.0f, 250), UDim(0.0f, 5)));
        element->setText("0");
    }

    Window* element = 0;

    element = wmgr.getWindow(winName);
    element->setText(smartass);

    element = wmgr.getWindow(winName + "Smartass");
    element->setText(name);

    element = wmgr.getWindow(winName + "X");
    element->setText(Ogre::StringConverter::toString(x));

    element = wmgr.getWindow(winName + "Y");
    element->setText(Ogre::StringConverter::toString(y));

    element = wmgr.getWindow(winName + "Z");
    element->setText(Ogre::StringConverter::toString(z));
}
