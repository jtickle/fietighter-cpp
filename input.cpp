#include "input.h"
#include "OgreStringConverter.h"
#include "Player.h"
#include "GUI.h"
#include "GameBoard.h"
#include "FTLog.h"

using namespace Ogre;
using namespace OIS;

InputHandler::InputHandler(RenderWindow* window, GameBoard* gameboard, GUI* gui) : mGameBoard(gameboard), mGUI(gui) {
    size_t windowHnd = 0;
    ParamList pl;
    window->getCustomAttribute("WINDOW", &windowHnd);
    pl.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(windowHnd)));

    mMouse    = 0;
    mKeyboard = 0;
    mJoystick = 0;

    m_hWnd = windowHnd;
    m_ois = OIS::InputManager::createInputSystem(pl);
    mMouse = static_cast<OIS::Mouse*>(m_ois->createInputObject(OIS::OISMouse, true));
    mKeyboard = static_cast<OIS::Keyboard*>(m_ois->createInputObject(OIS::OISKeyboard, true));
    try {
        mJoystick = static_cast<OIS::JoyStick*>(m_ois->createInputObject(OIS::OISJoyStick, true));
        cout << "Joystick Detected!\n";
    } catch (const OIS::Exception &exception) {
        cout << "No Joystick Detected.\n";
    }
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
    if(mJoystick)
        mJoystick->setEventCallback(this);

    terminate = false;
}

InputHandler::~InputHandler() {
    if(mMouse)
        m_ois->destroyInputObject(mMouse);
    if(mKeyboard)
        m_ois->destroyInputObject(mKeyboard);
    if(mJoystick)
        m_ois->destroyInputObject(mJoystick);

    OIS::InputManager::destroyInputSystem(m_ois);
}

void InputHandler::capture() {
    mMouse->capture();
    mKeyboard->capture();
    if(mJoystick)
        mJoystick->capture();
}

void InputHandler::setPlayer(Player *p) {
    mPlayer = p;
}

void InputHandler::setWindowExtents(int width, int height) {
    // Set Mouse Region... if window resizes, we should alter this to reflect as well
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
}

// MouseListener
bool InputHandler::mouseMoved(const OIS::MouseEvent &evt) {
    mGUI->mouseMoved(evt.state.X.rel, evt.state.Y.rel);
    return true;
}

bool InputHandler::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID btn) {
    mGUI->mousePressed(btn);
    return true;
}

bool InputHandler::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID btn) {
    mGUI->mouseReleased(btn);
    return true;
}

// KeyListener
bool InputHandler::keyPressed(const OIS::KeyEvent &evt) {
    switch(evt.key) {
        case OIS::KC_BACK:
            mPlayer->setSpeed(1.0);
            break;
        case OIS::KC_RBRACKET:
            mPlayer->setSpeed(0.67);
            break;
        case OIS::KC_LBRACKET:
            mPlayer->setSpeed(0.33);
            break;
        case OIS::KC_BACKSLASH:
            mPlayer->setSpeed(0.0);
            break;

        case OIS::KC_C:
            mGameBoard->stopAllPlayers();
            break;
        case OIS::KC_G:
            mGameBoard->goAllPlayers();
            break;

        case OIS::KC_W:
        case OIS::KC_UP:
            mPlayer->setPitch(-1.0);
            break;

        case OIS::KC_S:
        case OIS::KC_DOWN:
            mPlayer->setPitch(1.0);
            break;

        case OIS::KC_A:
        case OIS::KC_LEFT:
            mPlayer->setYaw(1.0);
            break;

        case OIS::KC_D:
        case OIS::KC_RIGHT:
            mPlayer->setYaw(-1.0);
            break;

        case OIS::KC_Q:
            mPlayer->setRoll(-1.0);
            break;

        case OIS::KC_E:
            mPlayer->setRoll(1.0);
            break;

        case OIS::KC_SPACE:
            mPlayer->fireWeapon();
            break;

    }
    return true;
}

bool InputHandler::keyReleased(const OIS::KeyEvent &evt) {
    switch(evt.key) {
        case OIS::KC_ESCAPE:
            terminate = true;
            break;

        case OIS::KC_W:
        case OIS::KC_UP:
        case OIS::KC_S:
        case OIS::KC_DOWN:
            mPlayer->setPitch(0.0);
            break;

        case OIS::KC_A:
        case OIS::KC_LEFT:
        case OIS::KC_D:
        case OIS::KC_RIGHT:
            mPlayer->setYaw(0.0);
            break;

        case OIS::KC_Q:
        case OIS::KC_E:
            mPlayer->setRoll(0.0);
            break;
    }

    return true;
}

// JoyStickListener
bool InputHandler::buttonPressed(const OIS::JoyStickEvent &evt, int index) {
    switch(index) {
    case 1: // Roll Toggle
        mPlayer->setYaw(0);
        break;
    case 8: // Down Throttle
        mPlayer->setSpeed(0);
        break;
    case 9: // Up Throttle
        mPlayer->setSpeed(1);
        break;
    case 7: // FIRE AT WILL!
        mPlayer->fireWeapon();
        break;
    default:
        FTLog << "Button Pressed: " << index << std::endl;
        break;
    }
    return true;
}

bool InputHandler::buttonReleased(const OIS::JoyStickEvent &evt, int index) {
    switch(index) {
    case 1: // Roll Toggle
        mPlayer->setRoll(0);
        break;
    case 8: // Down Throttle
        break;
    case 9: // Up Throttle
        break;
    default:
        FTLog << "Button Released: " << index << std::endl;
    }
    return true;
}

bool InputHandler::axisMoved(const OIS::JoyStickEvent &evt, int index) {
    // 0 = Left Stick X
    // 1 = Left Stick Y
    // 2 = Right Stick X
    // 3 = Right Stick Y

    // Input coordinates don't match up properly with how things work in our
    // cartesian coordinate system, so throw some negatives in there to make it work
    if(evt.state.buttons & 2) {
        mPlayer->setRoll(-analogToPercent(evt.state.mAxes[0].abs));
    } else {
        mPlayer->setYaw(-analogToPercent(evt.state.mAxes[0].abs));
        mPlayer->setPitch(analogToPercent(evt.state.mAxes[1].abs));
    }
    return true;
}

bool InputHandler::povMoved(const OIS::JoyStickEvent &evt, int index) {
    FTLog << "Pov " << index << ": " << evt.state.mPOV[index].direction << std::endl;
    return true;
}

/**
 * Convert OIS analog data to a percentage rate for the player to turn.
 * Also figures in a certain tolerance for a stick to not exactly be
 * centered, but still return 0.
 */
Ogre::Real InputHandler::analogToPercent(int analog)
{
    if(analog > -ANALOG_TOLERANCE && analog < ANALOG_TOLERANCE)
        return 0;
    return (Ogre::Real)analog / OIS::JoyStick::MAX_AXIS;
}
