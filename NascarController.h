#ifndef NASCARCONTROLLER_H_DEFINED
#define NASCARCONTROLLER_H_DEFINED

#include "PlayerController.h"

class NascarController : public PlayerController
{
public:
    NascarController();
    virtual ~NascarController();

    void doFrame(Ogre::Real fracSec);

};

#endif // NASCARCONTROLLER_H_DEFINED
