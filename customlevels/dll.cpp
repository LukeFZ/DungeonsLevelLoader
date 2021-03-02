//#include <Windows.h>
#include <stdio.h>
#include <thread>

#include <unrealspy.h>
#include <uhook.h>
#include <offsets.h>
#include <console.h>
#include <fstream>
#include <utility>
#include "json.hpp"

#include <functional>
#include <iostream>
#include <map>
#include <ctime>
#include <chrono>
#include <codecvt>
#include <string>
#include <set>
#include <algorithm>
#include <vector>

#include "ui.h"
#include "dll.h"

spy::Data *spyData;
UIData uiData;
std::string dllPath;
FString SeedValueHUDString;
FString levelFile;

void Init();

// __int64 __fastcall subLevelLoad(__int64 a1, __int64 a2, char a3)
//typedef void (__fastcall *tLoadLevel)(UObject* thisBpGameInstance, FLevelSettings *params, uint8 r8b, double xmm3, DWORD64 stackFloat);
typedef void (__fastcall *tLoadLevel)(UObject*, FLevelSettings&, uint8_t, float, float, class APlayerController*, class FString&);
void LoadLevel(UObject* thisBpGameInstance, FLevelSettings& params, uint8_t mapLoadType, float fadeInTime, float fadeOutTime, class APlayerController* playerController, class FString& connString);

// SDK::UObject*,SDK::FLevelSettings&, SDK::EMapLoadType, float, float, class SDK::APlayerController*, struct SDK::FString&
//void AHUD_PostRender(void *hud);

tAHUD_DrawRect AHUD_DrawRect = nullptr;
tAHUD_DrawText AHUD_DrawText = nullptr;
tFStringTableRegistry_Internal_LocTableFromFile FStringTableRegistry_Internal_LocTableFromFile = nullptr;
tFName_Init FName_Init = nullptr;

UFont *font = nullptr;

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved) {

    if (dwReason == DLL_PROCESS_ATTACH) {


        char dllFilename[MAX_PATH];
        DWORD len = GetModuleFileNameA(hinst, (LPSTR)&dllFilename, sizeof(dllFilename));
        if (len == 0) {
            printf("No dll filename?\n");
            return 0;
        }
        std::string filename = dllFilename;
        size_t pos = filename.find_last_of("/\\");
        dllPath = filename.substr(0,pos);

        AllocConsole();
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
    
        // Init in separate thread.
        // Works in a dll as long as we don't wait for thread to join
        std::thread t(Init);
        t.detach();

    }
    else if (dwReason == DLL_PROCESS_DETACH) {

        // bool result = true;//DeInitSpy(&spyData);

        // printf("detach %s\n", result ? "successful" : "failed");
    }
    return 1;
}

std::wstring makeWide(std::string text) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(text);
}

bool hookedCall;

void LoadLevel(UObject* thisBpGameInstance, FLevelSettings& params, uint8_t mapLoadType, float fadeInTime, float fadeOutTime, class APlayerController* playerController, class FString& connString) {
    if (params.MissionState.Seed != 0) {
        hookedCall = true;
        printf("LevelLoad\n");
        // std::cout << "Name: " << (wchat_t*)params->levelName.
        //printf("  Name: %ws\n", (wchar_t*)params->levelName.GetData());
        printf("  Seed: %i\n", params.MissionState.Seed);
        //SeedValueHUDString = FString(makeWide(std::to_string(params.MissionState.Seed)).c_str());

            auto element = uiData.seeds.find(std::to_string(params.MissionState.Seed));
            if (element != uiData.seeds.end()) {

            printf(" Hooked Call!\n");
            FLevelSettings modified = params;
            std::wstring newfile = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(element->second.c_str());
            modified.levelFilename = FString(newfile.c_str());
            //modified.levelName = ELevelNames::ELevelNames__ELevelNames_MAX;

            std::string origLevelFileString(TCHAR_TO_UTF8(*modified.levelFilename));
            printf("New Level Filename: %s\n", origLevelFileString.c_str());
            hookedCall = false;
 
            modified.MissionState.Seed = rand() % 35001;

            printf("New Seed: %i\n",modified.MissionState.Seed);


            ((tLoadLevel)spy::GetHook(RefLoadLevel)->original)(thisBpGameInstance, modified, mapLoadType, fadeInTime, fadeOutTime, playerController, connString);
            return;
        }
    }
    ((tLoadLevel)spy::GetHook(RefLoadLevel)->original)(thisBpGameInstance, params, mapLoadType, fadeInTime, fadeOutTime, playerController, connString);
    return;
}

void AHUD_PostRender(void *hud) {

    FLinearColor textColor {1.0, 0.0, 0.0, 1.0};
    FLinearColor bg {0.1, 0.1, 0.1, 0.6};

    AHUD_DrawRect(hud, bg, 20.0, 20.0, 200.0, 100.0);

    FString CheatHUDString = FString(L"Random Seed mode enabled!");
    FString SeedHUDString = FString(L"Seed:");

    AHUD_DrawText(hud,&CheatHUDString, textColor, 30, 30, font, 1, false);
    AHUD_DrawText(hud,&SeedHUDString, textColor, 30, 45, font, 1, false);
    AHUD_DrawText(hud,&SeedValueHUDString, textColor, 80, 45, font, 1, false); 
    ((tAHUD_PostRender)spy::GetHook(RefAHUD_PostRender)->original)(hud);
    return;
}


// Initializing in a separate thread
void Init() {
    
     uiData.seeds = {};
     srand(time(NULL));

    // Read launcher_settings.json for Minecraft Dungeons path
    using json = nlohmann::json;
    std::string filename = "customlevels.json";

    printf("Reading %s\\%s\n", dllPath.c_str(), filename.c_str());
    std::ifstream in(dllPath + "\\" + filename);

    if (in.is_open() == true) {
        json j;
        in >> j;
        
        // It's ordered by key.. Unfortunately
        uiData.seeds = j.get<std::map<std::string,std::string>>();

        /*uiData.seeds.clear();

        for(auto &entry: uiData.seeds) {
            printf("%s\n", entry.first.c_str());
            uiData.seeds[entry.first] = entry.second;
        }*/

        in.close();
    } else {
        printf("File not found\n");
    }


    StartUI(&uiData);

    spyData = spy::Init(offsets::defaultAddressLookups);
    if (spyData == nullptr) {
        printf("Unable to init Unreal Spy\n");
        return;
    }

    spy::HookFunctionRef(RefLoadLevel, &LoadLevel, nullptr);
    //spy::HookFunctionRef(RefAHUD_PostRender, &AHUD_PostRender, nullptr);
    //AHUD_DrawRect = (tAHUD_DrawRect)spyData->functionPtrs["AHUD_DrawRect"];
    //AHUD_DrawText = (tAHUD_DrawText)spyData->functionPtrs["AHUD_DrawText"];
    FName_Init = (tFName_Init)spyData->functionPtrs["RefFName_Init"];
    FStringTableRegistry_Internal_LocTableFromFile = (tFStringTableRegistry_Internal_LocTableFromFile)spyData->functionPtrs["FStringTableRegistry_Internal_LocTableFromFile"];

    spy::InitConsole();
        bool result = spy::InitCheatCommands([](bool result) {
        //printf("Cheat commands disabled!\n");
    });
    if (result == false) {
        printf("No cheat commands\n");
    }

    /*FName test;
    FName_Init(&test,L"op",0,1,false,-1);
    printf("FName created!\n");

    FString labels = FString(L"opLabels");
    FString pathlabel = FString(L"Decor/Text/opLabels.csv");

    FStringTableRegistry_Internal_LocTableFromFile(test, labels, labels, pathlabel);*/


}
