#include "HumanController.h"
#include "PlayerController.h"

#include "Player.h"
#include "GUI.h"

#include "Ogre.h"
using namespace Ogre;

#include <cmath>

HumanController::HumanController(GUI* gui) : PlayerController(), mGUI(gui), mTarget(NULL)
{
}

HumanController::~HumanController()
{
}

void HumanController::doFrame(Ogre::Real fracSec)
{
    if(!mPlayer) return;

    // Try to figure out what direction I am facing
    /*if(mTarget) {
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

        std::cout << turnTo.x << "\t" << turnTo.y << "\t" << turnTo.z << "\t" << pitch << "\t" << yaw << "\n";

        mPlayer->setPitch(pitch);
        mPlayer->setYaw(yaw);
    } else {
        FindClosestVisitor* v = new FindClosestVisitor(mPlayer);
        mPlayerMgr->visitAllPlayers(v);

        if(v->closest) {
            Radian r = mPlayer->getAngleTo(v->closest);
            mGUI->humanStats("Searching...", v->closest->getName(),
                mPlayer->getAngleTo(v->closest).valueRadians(),
                0.0, 0.0);
            if(r.valueDegrees() < 60) {
                mTarget = v->closest;
            }
        }

        delete v;
    }*/
}

void HumanController::destroyNotify(Player* p)
{
    if(mTarget == p) {
        mTarget = NULL;
    }
}
