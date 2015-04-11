#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include "OIS/OISEvents.h"
#include "OIS/OISInputManager.h"
#include "OIS/OISMouse.h"
#include "OIS/OISKeyboard.h"
#include "OIS/OISJoyStick.h"
#include "Ogre.h"
#include "CEGUI.h"

class GUI
{
public:
    GUI(CEGUI::Renderer* renderer);
    void mousePressed(OIS::MouseButtonID id);
    void mouseReleased(OIS::MouseButtonID id);
    void mouseMoved(Ogre::Real x, Ogre::Real y);
    void displaySpeed(Ogre::String speed);
    void displayStats(Ogre::String name, Ogre::String state, Ogre::String target);
    void humanStats(Ogre::String smartass, Ogre::String name, Ogre::Real x, Ogre::Real y, Ogre::Real z);
    void displayKill(Ogre::String message);
protected:
    void createInGameLayout();
    void createStatsWindow(const Ogre::String& name, const Ogre::String& title);
    CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);
    CEGUI::System* mSystem;
    CEGUI::Renderer* mRenderer;
    CEGUI::Window* mInGameWindow;
    CEGUI::Window* mSpeedDisplay;
    CEGUI::Window* mLogWindow;
    int mStatsPos;
    int mLogPos;
};

#endif // GUI_H_INCLUDED
