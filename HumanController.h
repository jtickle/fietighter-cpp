#ifndef HUMANCONTROLLER_H_DEFINED
#define HUMANCONTROLLER_H_DEFINED

#include "PlayerController.h"
#include "SmartController.h"

class HumanController : public PlayerController
{
public:
    HumanController(GUI* gui);
    virtual ~HumanController();

    void doFrame(Ogre::Real fracSec);
    void destroyNotify(Player* p);

protected:
    Player* mTarget;
    GUI*    mGUI;

};

#endif // HUMANCONTROLLER_H_DEFINED
