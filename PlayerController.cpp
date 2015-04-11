#include "PlayerController.h"
#include "PlayerManager.h"
#include "Player.h"

PlayerController::PlayerController() : mPlayer(NULL)
{
}

void PlayerController::setPlayer(Player* p)
{
    mPlayer = p;
    mPlayer->setSpeed(1);
}

void PlayerController::setPlayerManager(PlayerManager* pm)
{
    mPlayerMgr = pm;
}

void PlayerController::forgetPlayer()
{
    mPlayer = NULL;
}

void PlayerController::destroyNotify(Player* p)
{
}
