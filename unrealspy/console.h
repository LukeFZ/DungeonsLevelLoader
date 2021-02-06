#pragma once
#include "unrealspy.h"

namespace spy {
    bool InitConsole();
    bool InitCheatCommands(std::function<void (bool result)> fnResult);
    bool RunConsoleCommand(FString command);
}
void __stdcall FConsoleManager_ProcessUserConsoleInput(FConsoleManager* thisConsoleManager, const TCHAR* InInput, void *Ar, void *InWorld);
//bool InitConsole(SpyData *data);

