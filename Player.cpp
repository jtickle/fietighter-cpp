#include "Player.h"
#include "GameBoard.h"
#include "Ogre.h"
#include "FTLog.h"

using namespace Ogre;

Player::Player(SceneNode* sceneNode, GameBoard* gameBoard, const String name, bool showModel)
{
    mSceneNode = sceneNode;
    mGameBoard = gameBoard;
    mName = name;

    mSceneNode->setUserAny(Any(this));

    // These values could be set by a game script.  You might even want
    // to do that for the rotational ones - after something blows up in
    // a cutscene, set mInertialRoll to something really high, and it'll
    // slow itself down as the mission starts up, might be a cool effect.
    mSetSpeed      = 0;
    mInertialSpeed = 0;
    mSetPitch      = 0;
    mInertialPitch = 0;
    mSetRoll       = 0;
    mInertialRoll  = 0;
    mSetYaw        = 0;
    mInertialYaw   = 0;

    // These values should be loaded from a config file.  They are what
    // make each ship handle differently.  Also important for capships
    // and missiles and even stationary objects that can be destroyed.
    // For now, to get this project turned in, we'll just hard-code.
    mMaxSpeed  = 600;
    mSpeedStep = 0.25;
    if(showModel) {
        mMaxPitch  = 90;
        mMaxRoll   = 90;
        mMaxYaw    = 90;
        mPitchStep = 40.0;
        mRollStep  = 40.0;
        mYawStep   = 40.0;
    } else {
        mMaxPitch  = 180;
        mMaxRoll   = 180;
        mMaxYaw    = 180;
        mPitchStep = 4.0;
        mRollStep  = 4.0;
        mYawStep   = 4.0;
    }

    if(showModel) {
        Entity* entity = mGameBoard->getSceneManager()->createEntity(name + "Ship", "ship.mesh");
        mSceneNode->attachObject(entity);
        /*ManualObject *manual = mGameBoard->getSceneManager()->createManualObject(name + "ZAxis");
        manual->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_LIST);
        manual->colour(ColourValue::Green);
        manual->position(0.0,0.0,0.0);
        manual->position(0.0,0.0,-50.0);
        manual->end();
        mSceneNode->attachObject(manual);*/
    } else {
        ManualObject *manual = mGameBoard->getSceneManager()->createManualObject(name + "Reticle");
        manual->begin("BaseWhiteNoLighting", RenderOperation::OT_LINE_LIST);
        manual->colour(ColourValue::Green);
        manual->position( -1.0,  0.0, -20.0 );
        manual->position( -0.2,  0.0, -20.0 );
        manual->position(  1.0,  0.0, -20.0 );
        manual->position(  0.2,  0.0, -20.0 );
        manual->position(  0.0, -1.0, -20.0 );
        manual->position(  0.0, -0.2, -20.0 );
        manual->position(  0.0,  1.0, -20.0 );
        manual->position(  0.0,  0.2, -20.0 );
        manual->end();
        mSceneNode->attachObject(manual);
    }

    mSceneQuery = mGameBoard->getSceneManager()->createRayQuery(Ray());
}

Player::~Player()
{
    mGameBoard->getSceneManager()->destroyQuery(mSceneQuery);
    mGameBoard->getSceneManager()->destroySceneNode(mSceneNode);
}

void Player::attachCamera(Camera* camera)
{
    mSceneNode->attachObject(camera);
}

void Player::setSpeed(Real pct)
{
    mSetSpeed = pct;
}

int Player::getActualSetSpeed()
{
    return (int)(mSetSpeed*mMaxSpeed);
}

int Player::getActualInertialSpeed()
{
    return (int)(mInertialSpeed*mMaxSpeed);
}

String Player::getDisplaySpeed()
{
    char display[15];
    sprintf(display, "%d/%d", (int)(mSetSpeed * mMaxSpeed),
        (int)(mInertialSpeed * mMaxSpeed));

    return String(display);
}

void Player::setPitch(Real pct)
{
    mSetPitch = pct;
}

Real Player::getSetPitch()
{
    return mSetPitch;
}

Real Player::getInertialPitch()
{
    return mInertialPitch;
}

void Player::setRoll(Real pct)
{
    mSetRoll = pct;
}

Real Player::getSetRoll()
{
    return mSetRoll;
}

Real Player::getInertialRoll()
{
    return mInertialRoll;
}

void Player::setYaw(Real pct)
{
    mSetYaw = pct;
}

Real Player::getSetYaw()
{
    return mSetYaw;
}

Real Player::getInertialYaw()
{
    return mInertialYaw;
}

String Player::getName()
{
    return mName;
}

void Player::doFrame(Real fracSecond)
{
    updateInertialValues(fracSecond);

    // Update Weapon Cooldown Time
    mWeaponCooldown -= fracSecond;
    if(mWeaponCooldown <= 0) {
        mWeaponCooldown = 0;
    }

    // Cartesian degrees go counter-clockwise.  We want this for Pitch and nothing else.
    Real fracPitch = sinceLastFrame(fracSecond,  mInertialPitch, mMaxPitch);
    Real fracRoll  = sinceLastFrame(fracSecond,  mInertialRoll,  mMaxRoll);
    Real fracYaw   = sinceLastFrame(fracSecond,  mInertialYaw,   mMaxYaw);
    // Negative Z goes into the screen
    Real fracSpeed = sinceLastFrame(fracSecond, -mInertialSpeed, mMaxSpeed);

    mSceneNode->pitch(Degree(fracPitch), Node::TS_LOCAL);
    mSceneNode->roll (Degree(fracRoll),  Node::TS_LOCAL);
    mSceneNode->yaw  (Degree(fracYaw),   Node::TS_LOCAL);

    mSceneNode->translate(0, 0, fracSpeed,     Node::TS_LOCAL);
}

Real Player::sinceLastFrame(Real fracSecond, Real pct, int max)
{
    return  fracSecond * pct * max;
}

/**
 * Provides a weak attempt at inertia / drag.  This can be configured with different
 * values for the "mXStep" variables.  With "Inertia", if you "let go" of the stick,
 * you will gradually come to a stop instead of just instantly halting.  This is more
 * important for Speed than anything else, but it provides a sort of feel for how a
 * ship "handles" if done right for the rotational options.
 */
void Player::updateInertialValues(Real fracSecond)
{
    mInertialSpeed = inertiaUtility(fracSecond, mSetSpeed, mInertialSpeed,
        1,  0, mSpeedStep);
    mInertialPitch = inertiaUtility(fracSecond, mSetPitch, mInertialPitch,
        1, -1, mPitchStep);
    mInertialRoll  = inertiaUtility(fracSecond, mSetRoll,  mInertialRoll,
        1, -1, mRollStep);
    mInertialYaw   = inertiaUtility(fracSecond, mSetYaw,   mInertialYaw,
        1, -1, mYawStep);

    if(mName == "Player1Human") {
        //OverlayElement* stats = OverlayManager::getSingleton().getOverlayElement("FieTighter/DebugOverlay/Player1Stats");

        //std::ostringstream dbgStream;
        //dbgStream << "Player 1:" << mInertialSpeed << "\t" << mInertialPitch << "\t" << mInertialYaw << "\t" << mInertialRoll;
        //stats->setCaption(dbgStream.str());
    }
}

/**
 * Updates Inertia for the provided inertial values.
 */
Real Player::inertiaUtility(Real fracSecond, Real set,
        Real inertial, Real max, Real min, Real step)
{
    // calculate the stepping for a fractional second
    Real perFrac = step * fracSecond;

    // If set and inertial are less than perFrac apart...
    if(std::abs(set - inertial) < perFrac) {
        inertial = set; // set and inertial are close enough.
    } else if(set < inertial) { // inertial is decreasing
        inertial -= perFrac;
    } else if(set > inertial) { // inertial is increasing
        inertial += perFrac;
    }

    if(inertial > max) inertial = max;  // Constrain to maximum
    if(inertial < min) inertial = min;  // Constrain to minimum

    return inertial;
}

bool Player::canFire()
{
    return mWeaponCooldown == 0;
}

void Player::fireWeapon()
{
    Vector3 pos = mSceneNode->getPosition();
    Vector3 dir = mSceneNode->getOrientation() * -Vector3::UNIT_Z;
    Ray laserBeam(pos, dir);

    // debug time
    Vector3 beam = laserBeam.getPoint(100);

    mSceneQuery->setRay(laserBeam);
    RaySceneQueryResult &result = mSceneQuery->execute();
    RaySceneQueryResult::iterator itr = result.begin();
    for(; itr != result.end(); ++itr)
    {
        // Rule some things out
        if(!itr->movable) continue;
        if(itr->movable->getMovableType() != "Entity") continue;

        Player* p = any_cast<Player*>(itr->movable->getParentSceneNode()->getUserAny());
        if(p == this) continue;

        mGameBoard->blowUp(this, p);
    }

    // TODO: Draw a laser beam

    mWeaponCooldown = WEAPON_COOLDOWN;
}

const Vector3& Player::getPosition()
{
    return mSceneNode->getPosition();
}

const Quaternion& Player::getOrientation()
{
    return mSceneNode->getOrientation();
}

Radian Player::getAngleTo(Player* p)
{
    return (mSceneNode->getOrientation() * -Vector3::UNIT_Z).angleBetween(
        getRelativePositionOf(p).normalisedCopy());
}

Quaternion Player::getRotationTo(Player* p)
{
    return (mSceneNode->getOrientation() * -Vector3::UNIT_Z).getRotationTo(
        getRelativePositionOf(p));
}

Vector3 Player::getRelativePositionOf(Player* p)
{
    // Looks like convertWorldToLocalPosition doesn't exist yet, so I'm
    // paraphrasing this code out of a newer version of OgreNode.cpp.
    return mSceneNode->_getDerivedOrientation().Inverse() *
        (p->getPosition() - mSceneNode->_getDerivedPosition()) /
        mSceneNode->_getDerivedScale();
}
