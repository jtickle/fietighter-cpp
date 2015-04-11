#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "Ogre.h"

#include "OIS/OISEvents.h"
#include "OIS/OISInputManager.h"
#include "OIS/OISMouse.h"
#include "OIS/OISKeyboard.h"
#include "OIS/OISJoyStick.h"
#include "OIS/OISException.h"

#define ANALOG_TOLERANCE 513
// Forward Declarations
class Player;
class GUI;
class GameBoard;

class InputHandler :
    public OIS::MouseListener,
    public OIS::KeyListener,
    public OIS::JoyStickListener
{
private:
    OIS::InputManager* m_ois;
    OIS::Mouse*        mMouse;
    OIS::Keyboard*     mKeyboard;
    OIS::JoyStick*     mJoystick;
    unsigned long      m_hWnd;
    Player*            mPlayer;
    bool               terminate;
    GUI*               mGUI;
    GameBoard*         mGameBoard;

public:
    InputHandler(Ogre::RenderWindow* window, GameBoard* gameboard, GUI* gui);
    ~InputHandler();

    void setWindowExtents(int width, int height);
    void capture();

    void setPlayer(Player *p);

    bool requestedTerminate() { return terminate; }

    // MouseListener
    bool mouseMoved(const OIS::MouseEvent &evt);
    bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID);
    bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID);

    // KeyListener
    bool keyPressed(const OIS::KeyEvent &evt);
    bool keyReleased(const OIS::KeyEvent &evt);

    // JoySticListener
    bool buttonPressed(const OIS::JoyStickEvent &evt, int index);
    bool buttonReleased(const OIS::JoyStickEvent &evt, int index);
    bool axisMoved(const OIS::JoyStickEvent &evt, int index);
    bool povMoved(const OIS::JoyStickEvent &evt, int index);

protected:
    Ogre::Real analogToPercent(int analog);
};

#endif // INPUT_H_INCLUDED
