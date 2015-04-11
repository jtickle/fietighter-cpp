#include "PlayerController.h"
#include "SmartController.h"
#include "Player.h"
#include "Radar.h"
#include "FTLog.h"

#include "OgreTimer.h"
#include "Ogre.h"

#include <cmath>

using namespace Ogre;

#define HUNT_THRESHOLD 60
#define ATTACK_THRESHOLD 0.01

SmartController::SmartController() : PlayerController()
{
    mState = PS_HUNTING;
    mTimer = new Timer();
    mTarget = NULL;
}

SmartController::~SmartController()
{
    delete mTimer;
}

Ogre::String SmartController::getStateString()
{
    switch(mState) {
        case PS_HUNTING:
            return "Hunting";
        case PS_TRACKING:
            return "Tracking";
        case PS_ATTACKING:
            return "Attacking";
        case PS_EVADING:
            return "Evading";
    }

    return "UNKNOWN??";
}

Player* SmartController::getTarget()
{
    return mTarget;
}

bool SmartController::hasTarget()
{
    return mTarget != NULL;
}

void SmartController::destroyNotify(Player* p)
{
    if(mTarget == p) {
        mTarget = NULL;
        mState = PS_HUNTING;
    }

    FTLog << "Notified " << mPlayer->getName() << "\n";
    FTLog.flush();
}

void SmartController::doFrame(Ogre::Real fracSec)
{
    if(!mPlayer) return;

    switch(mState) {
    default:
    case PS_HUNTING:
        doHunt(fracSec);
        break;
    case PS_TRACKING:
        doTrack(fracSec);
        break;
    case PS_ATTACKING:
        doAttack(fracSec);
        break;
    case PS_EVADING:
        doEvade(fracSec);
        break;
    }
}

void SmartController::doHunt(Ogre::Real fracSec)
{
    FindClosestVisitor* v = new FindClosestVisitor(mPlayer);
    mPlayerMgr->visitAllPlayers(v);

    if(v->closest) {
        Radian r = mPlayer->getAngleTo(v->closest);
        if(r.valueDegrees() < HUNT_THRESHOLD) {
            mTarget = v->closest;
            mState = PS_TRACKING;
            FTLog << mPlayer->getName() << " targeted " << v->closest->getName() << "\n";
            FTLog.flush();
        }
    }

    moveRandom(fracSec);

    delete v;
}

void SmartController::doTrack(Ogre::Real fracSec)
{
    Vector3 turnTo = mPlayer->getRelativePositionOf(mTarget);
    turnTo.normalise();     // The relative coordinates I need to turn to face

    Real yaw   = atan2(turnTo.z, turnTo.x);
    Real pitch = atan2(turnTo.y, sqrt(pow(turnTo.x,2) + pow(turnTo.z,2)));

    // For some reason this is off by 90*.  Correct that.
    yaw = fmod(yaw + (M_PI/2) + M_PI, M_PI * 2) - M_PI;

    // Unless you're almost pointed at the enemy, rcs should be at full.
    yaw = (-yaw * 4) / M_PI;
    pitch = (pitch * 4) / M_PI;

    if(yaw > 1.0) {
        yaw = 1.0;
    } else if(yaw < -1.0) {
        yaw = -1.0;
    }

    if(pitch > 1.0) {
        pitch = 1.0;
    } else if(pitch < -1.0) {
        pitch = -1.0;
    }

    mPlayer->setPitch(pitch);
    mPlayer->setYaw(yaw);
    mPlayer->setRoll(0);

    // Attack?
    if(mPlayer->canFire()) {
        // Are they within a reasonable tolerance of "in front"?
        if(pitch < ATTACK_THRESHOLD && yaw < ATTACK_THRESHOLD) {
            mPlayer->fireWeapon();
            FTLog << mPlayer->getName() << " fired upon " << mTarget->getName() << "\n";
            FTLog.flush();
        }
    }
}

void SmartController::doAttack(Ogre::Real fracSec)
{
}

void SmartController::doEvade(Ogre::Real fracSec)
{
}

void SmartController::moveRandom(Ogre::Real fracSec)
{
    static Ogre::Real fracCount = Ogre::Real(0);
    fracCount += fracSec;

    if(fracCount > 1.0) {
        fracCount = 0;

        Ogre::Real random = ((double)rand() / RAND_MAX) / 2 + 0.5;
        if(rand() > (RAND_MAX / 2)) random = -random;
        mPlayer->setYaw(random);

        random = ((double)rand() / RAND_MAX) / 2 + 0.5;
        if(rand() > (RAND_MAX / 2)) random = -random;
        mPlayer->setPitch(random);

        random = ((double)rand() * 2 / RAND_MAX) - RAND_MAX;
        mPlayer->setRoll(random);
    }
}

FindClosestVisitor::FindClosestVisitor(Player* player) : mPlayer(player)
{
    minRange = 999999;
    closest = NULL;
}

FindClosestVisitor::~FindClosestVisitor()
{
}

void FindClosestVisitor::visit(Player* p, PlayerController* pc)
{
    if(p == mPlayer) return;

    Real distance = mPlayer->getPosition().distance(p->getPosition());
    if(distance < minRange) {
        minRange = distance;
        closest = p;
    }
}
