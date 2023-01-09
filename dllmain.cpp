// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <Spore/App/IGameMode.h>
#include <Spore/Simulator/cEmpire.h>
#include <Spore/Simulator/cSimulatorSpaceGame.h>
#include "TaxCollectingStrategy.hpp"
#include <filesystem>
#include <Spore/Internal.h>

using namespace Simulator;
using namespace ArgScript;

class PlanetBusterCheat
    : public ICommand
{
public:
    void ParseLine(const Line& line) {       
        if (Simulator::IsSpaceGame()) {

            App::ConsolePrintF("Drop the bomb. BOOM!");

            cSpaceToolDataPtr tool;
            ToolManager.LoadTool({ id("PlanetBusterBomb"), 0, 0 }, tool);

            size_t numArgs;
            auto args = line.GetArgumentsRange(&numArgs, 0, 1);
            if (numArgs == 1) {
                tool->mCurrentAmmoCount = mpFormatParser->ParseInt(args[0]);
            }

            auto inventory = SimulatorSpaceGame.GetPlayerInventory();
            inventory->AddItem(tool.get());
            cEmpire* empire = GetPlayerEmpire();
            empire->mEmpireMoney += 100000;
        }

        else {
            App::ConsolePrintF("You need to be in the space stage to use this!");
            return;
        }
    }

    const char* GetDescription(DescriptionMode mode) const {
        return "Call the cheat. Drop the bomb. BOOM!";
    }
};

// Defender ships cheat.

class PlayerDefenders
    : public ICommand
{

public:
    void ParseLine(const Line& line) {
        if (Simulator::IsSpaceGame()) {

            App::ConsolePrintF("Defend!!!");
            cEmpire* empire = GetPlayerEmpire();
            auto flags = empire->mFlags;
            App::ConsolePrintF("%d", flags);
            empire->mFlags = 0x4;
            cEmpire* empire2 = GetPlayerEmpire();
            auto flags2 = empire2->mFlags;
            App::ConsolePrintF("%d", flags2);
        }

        else {
            App::ConsolePrintF("You need to be in the space stage to use this!");
            return;
        }
    }

    const char* GetDescription(DescriptionMode mode) const {
        return "Used to get defender ships.";
    }
};

void Initialize() {
    CheatManager.AddCheat("planetBuster", new PlanetBusterCheat());
    CheatManager.AddCheat("playerDefenders", new PlayerDefenders());

    int hi = 0;
    static TaxCollectingStrategy taxCollectingStrategy(hi);

    for (const uint32_t messageID : taxCollectingStrategy.GetHandledMessageIDs()) {
        MessageManager.AddUnmanagedListener(&taxCollectingStrategy, messageID);
    }

    SimulatorSystem.AddStrategy(&taxCollectingStrategy, id("SporeTaxCollecting"));

    //auto AddSimulatorStrategy(Simulator::ISimulatorStrategy * TaxCollectingStrategy, uint32_t taxCollectingStrategy)->MODAPI bool;
}

void Dispose()
{
	// This method is called when the game is closing
}

void AttachDetours()
{
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

