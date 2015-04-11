#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include <vector>
#include <map>
#include "Ogre.h"

typedef enum {
    STARTUP,
    GUI,
    LOADING,
    CANCEL_LOADING,
    SIMULATION,
    SHUTDOWN
} SimulationState;

class Simulation {

public:
    Simulation();
    virtual ~Simulation();

    bool requestStateChange(SimulationState state);
    bool lockState();
    bool unlockState();
    SimulationState getCurrentState();

    void setFrameTime(float ms);
    inline float getFrameTime() { return m_frame_time; }

protected:
    SimulationState m_state;
    bool m_locked;
    float m_frame_time;
};

#endif // SIMULATION_H_INCLUDED
