#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Ogre.h"

// Units Per Second
#define SPEED_INCREASE_BY 200
#define MAX_SPEED 600
#define MIN_SPEED 0

// Degrees Per Second
#define MAX_ROT_RATE 180

// Weapon Cooldown Time
#define WEAPON_COOLDOWN 0.5

// Forward Declarations
class GameBoard;

class Player
{
public:
    Player(Ogre::SceneNode* sceneNode, GameBoard* GameBoard, const Ogre::String name, bool showModel = true);
    virtual ~Player();

    virtual void doFrame(Ogre::Real fracSec);

    virtual void setSpeed(Ogre::Real pct);
    virtual int getActualInertialSpeed();
    virtual int getActualSetSpeed();

    virtual void setPitch(Ogre::Real pct);
    virtual Ogre::Real getSetPitch();
    virtual Ogre::Real getInertialPitch();

    virtual void setYaw(Ogre::Real pct);
    virtual Ogre::Real getSetYaw();
    virtual Ogre::Real getInertialYaw();

    virtual void setRoll(Ogre::Real pct);
    virtual Ogre::Real getSetRoll();
    virtual Ogre::Real getInertialRoll();

    virtual Ogre::String getName();
    virtual const Ogre::Vector3& getPosition();
    virtual const Ogre::Quaternion& getOrientation();
    Ogre::Radian getAngleTo(Player* p);
    Ogre::Quaternion getRotationTo(Player* p);
    virtual Ogre::Vector3 getRelativePositionOf(Player* p);

    virtual void attachCamera(Ogre::Camera* camera);

    Ogre::String getDisplaySpeed();

    virtual void fireWeapon();
    virtual bool canFire();

protected:
    Ogre::Real sinceLastFrame(Ogre::Real fracSecond, Ogre::Real pct, int max);
    Ogre::Real inertiaUtility(Ogre::Real fracSecond, Ogre::Real set,
        Ogre::Real inertial, Ogre::Real max, Ogre::Real min, Ogre::Real step);
    void updateInertialValues(Ogre::Real fracSecond);


    Ogre::SceneNode* mSceneNode;
    GameBoard* mGameBoard;
    Ogre::String mName;
    Ogre::RaySceneQuery* mSceneQuery;

    // Percentages make things easier than providing raw values.  This way, regardless
    // of the individual per-ship settings, pushing the stick halfway to the right
    // gives you a 50% of whatever is available.  Also, it makes it easier to switch
    // between different input types, so if you want to just use a keyboard, pressing
    // the "go" key gives you 100%, releasing gives you 0%.
    Ogre::Real mSetSpeed;       // Percentage of mMaxSpeed
    Ogre::Real mInertialSpeed;  // Percentage of mMaxSpeed, set internally
    Ogre::Real mSetPitch;       // Percentage of mMaxPitch
    Ogre::Real mInertialPitch;  // Percentage of mMaxPitch, set internally
    Ogre::Real mSetRoll;        // Percentage of mMaxRoll
    Ogre::Real mInertialRoll;   // Percentage of mMaxRoll, set internally
    Ogre::Real mSetYaw;         // Percentage of mMaxYaw
    Ogre::Real mInertialYaw;    // Percentage of mMaxYaw, set internally

    // The following things should be set by configuration; they're what makes
    // each ship/object in the game different
    int mMaxSpeed;         // Distance traveled in units per second
    Ogre::Real mSpeedStep; // How much to increase/decrease in percentage
    int mMaxPitch;         // Pitch rate in degrees per second
    Ogre::Real mPitchStep; // How much to increase/decrease in percentage
    int mMaxRoll;          // Roll rate in degrees per second
    Ogre::Real mRollStep;  // How much to increase/decrease in percentage
    int mMaxYaw;           // Yaw rate in degrees per second
    Ogre::Real mYawStep;   // How much to increase/decrease in percentage

    Ogre::Real mWeaponCooldown; // Limits so you can only fire once every WEAPON_COOLDOWN.
};

#endif // PLAYER_H_INCLUDED
