#include "stdafx.h"
#include "TaxCollectingStrategy.hpp"

#include <Spore/App/IGameMode.h>
#include <Spore/Simulator/cEmpire.h>
#include <Spore/Simulator/cSimulatorSpaceGame.h>

using namespace Simulator;
using namespace ArgScript;

int CollectionRate = 120;

void TaxCollectingStrategy::Update(int deltaTime, int deltaGameTime)
{
    if (Simulator::IsSpaceGame()) {

        if (IsPaused == false) {

            if (std::chrono::high_resolution_clock::now() > Timer) {
                Times += 1;
                Timer = std::chrono::high_resolution_clock::now() + std::chrono::seconds(1);

                if (Times > CollectionRate) {
                    Times = 0;

                    cEmpire* empire = GetPlayerEmpire();
                    Colonies = Simulator::SpacePlayerData::Get()->mPlayerColonies.size();

                    empire->mEmpireMoney += 20000 + (10000 * (Colonies - 1));
                }
                

            }
        }
    }

    else {
        return;
    }
}

void TaxCollectingStrategy::OnModeEntered(uint32_t previousModeID, uint32_t newModeID)
{
    switch (newModeID)
    {
    default:
        break;

    case GameModeIDs::kGameSpace:
        Times = 0;
        Timer = std::chrono::high_resolution_clock::now() + std::chrono::seconds(1);
        break;
    }
}

std::vector<uint32_t> TaxCollectingStrategy::GetHandledMessageIDs(void)
{
    std::vector<uint32_t> handledMessageIDs;

    handledMessageIDs.push_back((uint32_t)MessageID::OnPauseToggled);
    
    return handledMessageIDs;
}

bool TaxCollectingStrategy::HandleMessage(uint32_t messageID, void* message) {
    switch (messageID) {
    default:
        break;
    case (uint32_t)MessageID::OnPauseToggled:
        IsPaused = (message != nullptr);
        break;
    }

    return false;
}