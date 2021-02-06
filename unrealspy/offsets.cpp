
#include <cstdio>
#include <sstream>
#include "offsets.h"
#include <Windows.h>
#include <tlhelp32.h>

namespace offsets {

    // Some defaults for us.
    std::map<UE4Reference, std::string> defaultAddressLookups = {
        {RefFName_GetNames,                             "48 83 EC 28 ?? ?? ?? ?? ?? ?? ?? 48 85 C0 ?? ?? B9 08 08 00 00 48 89 5C 24 20"},
        // Sort of Ctor for FNames. Does the main work
        {"RefFName_Init",                                 "48 89 5C 24 18 55 56 57 48 81 EC 60 08 00 00 ?? ?? ?? ?? ?? ?? ?? 48 33 C4 48 89 84 24 50 08 00 00 0F B7 02"},
        {RefFRawObjectIterator_Ctor,                    "84 D2 48 C7 41 10 00 00 00 00 B8 FF FF FF FF ?? ?? ?? ?? ?? ?? ?? 89 41 08 4C 8B D1 4C 89 01"},

        // TODO These are UFunctions so we can just get them at runtime.
        {"AHUD_DrawRect",                               "48 8B C4 48 89 58 08 57 48 81 EC E0 00 00 00 0F 29 70 E8 48 8B FA 0F 29 78 D8 0F 28 F3 0F 28 FA 48 8B D9"},
        {"AHUD_DrawText",                               "40 55 56 57 48 81 EC 30 02 00 00 44 0F 29 84 24 00 02 00 00"},
        {"AHUD_GetTextSize",                            "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 40 49 8B F9 49 8B F0 48 8B EA 48 8B D9 ?? ?? ?? ?? ?? 84 C0"},

        {RefStaticLoadObject,                           "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 18 FE FF FF 48 81 EC E8 02 00 00 ?? ?? ?? ?? ?? ?? ?? 48 33 C4 48 89 85 D0 01 00 00 48 8B 85 68 02 00 00"},
        {RefStaticLoadClass,                            "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 08 FF FF FF 48 81 EC F8 01 00 00 ?? ?? ?? ?? ?? ?? ?? 48 33 C4 48 89 85 E0 00 00 00 8B BD 60 01 00 00"},
        {RefLoadPackage,                                "48 8B C4 53 56 48 83 EC 68 48 89 68 08 48 8B EA 4C 89 60 18 33 D2 4C 89 68 E8 4C 8B E1 4C 89 70 E0 48 8D 48 C8 4C 89 78 D8 45 33 ED"},
        {RefUObject_ProcessEvent,                       "40 55 56 57 41 54 41 55 41 56 41 57 48 81 EC F0 00 00 00 48 8D 6C 24 30 48 89 9D 18 01 00 00"},
        {RefAActor_ProcessEvent,                        "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B 01 49 8B F0 ?? ?? ?? ?? ?? ?? ?? 48 8B EA 48 8B D9"},
        {RefAHUD_PostRender,                            "40 55 56 48 8D 6C 24 B1 48 81 EC C8 00 00 00 48 8B 01 48 8B F1"},
        {RefStaticConstructObject_Internal,             "40 55 53 56 57 41 54 41 56 41 57 48 8D AC 24 50 FF FF FF 48 81 EC B0 01 00 00 ?? ?? ?? ?? ?? ?? ?? 48 33 C4 48 89 85 A8 00 00 00"},
        {RefUConsole_ConsoleCommand,                    "48 89 5C 24 10 48 89 6C 24 18 48 89 74 24 20 57 48 83 EC 30 48 8B F2 48 89 54 24 40 48 8B D9 48 8D 54 24 40 48 83 C1 68"},
        {RefFConsoleManager_ProcessUserConsoleInput,    "48 8B C4 4C 89 48 20 4C 89 40 18 48 89 48 08 55 56 48 8D 68 A1 48 81 EC B8 00 00 00 33 F6"},
        //{RefAddEmeralds,                              "40 55 56 57 48 81 EC 80 00 00 00 48 C7 44 24 30 FE FF FF FF 48 89 9C 24 B0 00 00 00 0F 29 74 24 70"},
         {RefLoadLevel,                                 "40 55 53 56 57 41 56 48 8D AC 24 70 FF FF FF 48 81 EC 90 01 00 00 48 C7 44 24 70 FE FF FF FF 0F 29 B4 24 80 01 00 00"},
         //{RefCreateLevelSettings                        "48 8B C4 4C 89 48 20 48 89 48 08 55 56 57 41 54 51 55 51 56 41 57 48 8D 68 B9 48 81 EC A0 00 00 00 48 C7 45 B7 FE FF FF"},
        //{RefLoadLevel,                                "40 55 53 56 57 41 54 41 56 41 57 48 8D AC 24 50 F6 FF FF 48 81 EC B0 0A 00 00 48 C7 44 24 60 Fe FF FF FF 48 8B 05 DE 9B 58"  },
//        {RefFSoftObjectPtr_LoadSynchronous,           "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 48 83 EC 20 48 8B F9 ?? ?? ?? ?? ?? 33 ED 48 8B F0 48 85 C0"},
        //{RefStaticLoadObject,                         "40 55 53 56 57 41 54 41 55 41 56 41 57 48 8D AC 24 18 FE FF FF 48 81 EC E8 02 00 00 ?? ?? ?? ?? ?? ?? ?? 48 33 C4 48 89 85 D0 01 00 00"},
        {RefUWorld_SpawnActor,                          "40 53 56 57 48 83 EC 70 ?? ?? ?? ?? ?? ?? ?? 48 33 C4 48 89 44 24 60 ?? ?? ?? ?? ?? ?? ?? 0F 57 D2 48 8B B4 24 B0 00 00 00 0F 28 CB"},
        {RefUUserWidget_CreateWidget,                   "48 89 5C 24 10 48 89 74 24 18 57 48 83 EC 30 49 8B D8 48 8B FA 48 8B F1 48 85 C9 ?? ?? ?? ?? ?? ?? 48 89 6C 24 40 48 8B A9 60 01 00 00 48 85 ED"},
        {RefUUserWidget_AddToViewport,                  "48 8B 01 44 8B C2 33 D2 48 FF A0 C8 02 00 00"},
        {RefFModuleManager_LoadModule,                  "48 89 5C 24 08 57 48 83 EC 20 ?? ?? ?? ?? ?? ?? ?? 48 8B DA 48 8B F9 ?? ?? ?? ?? ?? ?? ?? ?? 4C 8D 44 24 40 48 8B D3 48 8B CF"},
        {RefFModuleManager_LoadModuleWithFailureReason, "48 89 54 24 10 55 53 56 57 41 54 41 55 41 56 48 8D 6C 24 D9 48 81 EC B0 00 00 00"},
        {RefFModuleManager_Get,                         "48 83 EC 28 ?? ?? ?? ?? ?? ?? ?? 48 85 C0 ?? ?? ?? ?? ?? ?? 65 48 8B 04 25 58 00 00 00 8B 0D ?? ?? ?? ?? 41 B8 ?? ?? ?? ?? 48 8B 14 C8 41 8B 04 10 39 05 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8D"},
        {RefFWindowsPlatformProcess_GetDllHandle,       "40 55 53 56 48 8B EC 48 83 EC 40 33 F6 48 89 7C 24 68 4C 89 7C 24 78 4C 8B F9 48 8D 4D F0"},
        {RefUObject_CallFunctionByNameWithArguments,    "40 55 53 56 57 41 54 41 55 41 56 41 57 48 81 EC F8 02 00 00 48 8D 6C 24 30"},
        {"FStringTableRegistry_Internal_LocTableFromFile",     "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 57 41 54 41 55 41 56 41 57 48 83 ?? ?? 4D 8B E1 49 8B F8"},
    };

    // Lookup offsets using search strings and convert to runtime addresses.
    // lookups is a map like:
    // ["AHUD_DrawRect"] = "48 8B C4 48 89 ?? ?? 57 48 81 EC E0"
    // Returns a map keyed by the same string name and a uintptr_t which is the found address.
    std::map<UE4Reference,uintptr_t> FindAddresses(HANDLE process, std::map<UE4Reference,std::string> opcodes) {

        std::map<UE4Reference,uintptr_t> results;

        DWORD pid = GetProcessId(process);
        MODULEENTRY32 modEntry = GetModule(pid);
        if (modEntry.th32ModuleID == 0) {
            printf("Unable to find module. %ld\n", GetLastError());
            return results;
        }

        return FindAddresses(process, modEntry, opcodes);
    }

    std::map<UE4Reference,uintptr_t> FindAddresses(HANDLE process, MODULEENTRY32 modEntry, std::map<UE4Reference, std::string> opcodes) {

        struct Pattern {
            UE4Reference refName;
            BYTE values[128];
            char mask[128]; // Not null-terminated
            size_t length;
            bool found = false;
        };

        std::vector<Pattern> patterns;
        for (auto it = std::begin(opcodes); it != std::end(opcodes); ++it) {
            UE4Reference name = it->first;
            std::string opcode = it->second;

            Pattern pattern = {name};
            pattern.length = parseHex(opcode, &pattern.values[0], &pattern.mask[0]);

            patterns.push_back(pattern);
        }

        std::map<UE4Reference,uintptr_t> offsets;
        for (auto &it : patterns) {
            offsets[it.refName] = 0;
        }

        uintptr_t baseAddr = (uintptr_t)modEntry.modBaseAddr;
        uintptr_t endAddr = baseAddr + modEntry.modBaseSize;
        int numPatterns = patterns.size();

        for (MemoryIterator it(process, baseAddr, endAddr); it; ++it) {

            for(auto pit = std::begin(patterns); pit != std::end(patterns); ++pit) {

                Pattern pattern = *pit;
                if (pattern.found) {
                    continue;
                }

                int index = PatternScan(*it, it.GetBytesRead(), &pattern.values[0], &pattern.mask[0], pattern.length);

                if (index != -1) {
                    uintptr_t addr = it.GetCurrentAddr() + index;
                    offsets[pattern.refName] = addr;
                    pattern.found = true; // erase pattern instead?
                    numPatterns--;
                }
            }
            if (numPatterns == 0) {
                printf("all found\n");
                break;
            }
        }

        return offsets;
    }

    uintptr_t PatternScan(char* bytes, size_t size, const BYTE* pattern, const char* mask, size_t patternLength) {

        //printf("size %zd patlen %zd\n", size, patternLength);
        for (unsigned int i = 0; i <= size - patternLength; i++) {
            bool found = true;
            for (unsigned int j = 0; j < patternLength; j++) {
                if (mask[j] != '?' && pattern[j] != (BYTE)*(bytes + i + j)) {
                    found = false;
                    break;
                }
            }
            if (found) {
                return (uintptr_t)i;
            }
        }
        return -1;
    }

    // Convert IDA Pro binary search strings into an array of bytes and a mask
    // "48 8B ?? C4 53" = 0x48 0x8b 0x0 0xC4 mask = xx?x
    size_t parseHex(std::string hex, BYTE *bytes, char *mask) {

        std::istringstream ss(hex);
        std::string text;
        int i = 0;
        while (std::getline(ss, text, ' ')) {
            BYTE value = 0;
            if (text.c_str()[0] == '?') {
                mask[i] = '?'; 
            } else {
                mask[i] = 'x';
                value = strtol(text.c_str(), NULL, 16);
            }
            bytes[i] = value;

            i++;
        }
        return i;

    }




    // Get ModuleEntry from module name, using toolhelp32snapshot:
    // Now just gets the first module which is _probably_ the main one anyway.
    MODULEENTRY32 GetModule(const DWORD &pid) {
        MODULEENTRY32 modEntry = {0};

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);

        if (snapshot != INVALID_HANDLE_VALUE) {
            MODULEENTRY32 curr = {0};
            curr.dwSize = sizeof(MODULEENTRY32);

            if (Module32First(snapshot, &curr)) {
                modEntry = curr;
//                printf("%ws\n", curr.szModule);
                // do {
                //     if (strcmp((const char*)curr.szModule, modName) == 0) {
                //         modEntry = curr;
                //         break;
                //     }
                // } while (Module32Next(snapshot, &curr));
            }
            CloseHandle(snapshot);
        }
        return modEntry;
    }
}