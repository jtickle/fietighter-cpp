#ifndef STATIONARYCONTROLLER_H_DEFINED
#define STATIONARYCONTROLLER_H_DEFINED

#include "PlayerController.h"

class StationaryController : public PlayerController
{
public:
    StationaryController();
    virtual ~StationaryController();

    void doFrame(Ogre::Real fracSec);

};

#endif // STATIONARYCONTROLLER_H_DEFINED
