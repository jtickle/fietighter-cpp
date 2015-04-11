#include "StationaryController.h"
#include "PlayerController.h"

#include "Player.h"

#include <cmath>

StationaryController::StationaryController() : PlayerController()
{
}

StationaryController::~StationaryController()
{
}

void StationaryController::doFrame(Ogre::Real fracSec)
{
    if(!mPlayer) return;

    mPlayer->setSpeed(0);
}
