//#include "unrealspy.h"
//#include "util.h"
#include "console.h"
#include "helpers.h"
#include "uhook.h"
#include "unreal_impl.h"
//#include <windows.h>
#include <stdlib.h>
#include "console_autocomplete.h"

bool spy::InitConsole() {

    UGameViewportClient *GameViewport = GEngine->GameViewport;
    if (GameViewport == nullptr) {
        printf("no GameViewport\n");
        return false;
    }

    // // Have we already got a console?
    if (GameViewport->ViewportConsole != nullptr) {
        printf("no viewportconsole\n");
        return false;
    }

    // Hack to avoid triggering ::StaticClass() inside TSubclassOf.
    // TSubclassOf happens to have only one member, which is a pointer to the UClass.
    UClass *ConsoleClass = *(UClass**)&GEngine->ConsoleClass;
    
    FName NameNone;
    auto ConstructObject = GetFunction<tStaticConstructObject_Internal>(RefStaticConstructObject_Internal);
    UConsole *console = (UConsole*)ConstructObject(ConsoleClass, GameViewport, NameNone, RF_NoFlags, EInternalObjectFlags::None, nullptr, false, nullptr, false);
    if (console == nullptr) {
        printf("Unable to instantiate console class?\n");
        return false;
    }

    GameViewport->ViewportConsole = console;

    return true;
}


std::function<void (bool success)> fnEnableConsoleResult = 0;
tFConsoleManager_ProcessUserConsoleInput origProcessConsoleInput = nullptr;

bool spy::InitCheatCommands(std::function<void (bool result)> fnResult) {

    if (GEngine->GameViewport == nullptr || GEngine->GameViewport->ViewportConsole == nullptr) {
        return false;
    }


    bool result = spy::HookFunctionRef(RefFConsoleManager_ProcessUserConsoleInput, &FConsoleManager_ProcessUserConsoleInput, (void**)&origProcessConsoleInput);
    if (result == false) {
        printf("no console hook\n");
        return false;
    }

    fnEnableConsoleResult = fnResult;

    // Send random console command so we can get access to FConsoleManager.
    UConsole *console = GEngine->GameViewport->ViewportConsole;
    auto consoleCommand = GetFunction<tUConsole_ConsoleCommand>(RefUConsole_ConsoleCommand);
    FString str((TCHAR*)L"flat20");

    consoleCommand(console, &str);

    return true;
}

bool spy::RunConsoleCommand(FString command) {

    if (GEngine->GameViewport->ViewportConsole == nullptr) return false;
    UConsole *console = GEngine->GameViewport->ViewportConsole;
    auto consoleCommand = GetFunction<tUConsole_ConsoleCommand>(RefUConsole_ConsoleCommand);
    consoleCommand(console,&command);
    return true;
}

// Set the CVar flags to default to enable them.
static void OnConsoleVariable(const TCHAR *Name, IConsoleObject* CVar)
{
    //printf("%ws\n", Name);
    CVar->SetFlags(ECVF_Default);



    
}

void __stdcall FConsoleManager_ProcessUserConsoleInput(FConsoleManager* thisConsoleManager, const TCHAR* InInput, void *Ar, void *InWorld) {

    // Call original
    origProcessConsoleInput(thisConsoleManager, InInput, Ar, InWorld);

    // Unhook immediately? Or only after we've received the cheat command?
    if (wcscmp((const wchar_t*)InInput, L"flat20") != 0) { // TEXT("flat20")
        return;
    }


    bool result = spy::UnhookFunctionRef(RefFConsoleManager_ProcessUserConsoleInput);
    if (result == true) {
        origProcessConsoleInput = nullptr;
    }

    // Visit each console var to enable them.
    thisConsoleManager->ForEachConsoleObjectThatStartsWith(
        FConsoleObjectVisitor::CreateStatic(&OnConsoleVariable),
        (TCHAR*)L""
    );

    UConsole *console = spy::GEngine->GameViewport->ViewportConsole;
    autocomplete::BuildAutocomplete(thisConsoleManager, console);

    fnEnableConsoleResult(true);

}
