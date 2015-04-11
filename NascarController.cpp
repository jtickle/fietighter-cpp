#include "NascarController.h"
#include "PlayerController.h"

#include "Player.h"

#include <cmath>

NascarController::NascarController() : PlayerController()
{
}

NascarController::~NascarController()
{
}

void NascarController::doFrame(Ogre::Real fracSec)
{
    if(!mPlayer) return;

    static Ogre::Real fracCount = Ogre::Real(0);
    fracCount += fracSec;

    if(fmod(fracCount, Ogre::Real(2)) < 1) {
        mPlayer->setYaw(0);
    } else {
        mPlayer->setYaw(-1);
    }
}
