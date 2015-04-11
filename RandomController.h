#ifndef RANDOMCONTROLLER_H_DEFINED
#define RANDOMCONTROLLER_H_DEFINED

#include "PlayerController.h"

class RandomController : public PlayerController
{
public:
    RandomController();
    virtual ~RandomController();

    void doFrame(Ogre::Real fracSec);

};

#endif // RANDOMCONTROLLER_H_DEFINED
