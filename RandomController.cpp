#include "RandomController.h"
#include "PlayerController.h"

#include "Player.h"

#include <cmath>

RandomController::RandomController() : PlayerController()
{
}

RandomController::~RandomController()
{
}

void RandomController::doFrame(Ogre::Real fracSec)
{
    if(!mPlayer) return;

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
