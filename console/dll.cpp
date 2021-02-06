#include <Windows.h>
#include <stdio.h>

#include <unrealspy.h>
#include <offsets.h>
#include <util.h>
#include "ui.h"

signed int __stdcall hProcessEvent(UObject* object, UFunction* func, void* params);

SpyData spyData;
UIData uiData;

DWORD WINAPI UIThreadFunction(LPVOID lpParam);
HANDLE  hThread; 

BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved) {

    if (dwReason == DLL_PROCESS_ATTACH) {

        AllocConsole();
        freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

        HANDLE process = GetCurrentProcess();
        std::map<UE4Reference,uintptr_t> addresses = offsets::FindAddresses(process, offsets::defaultAddressLookups);

        spyData.detourPostRender = nullptr;
        spyData.detourProcessEvent = &hProcessEvent;
        InitSpy(&spyData, addresses);

        DWORD threadId; // Can we ignore this?
        hThread = CreateThread( NULL, 0, UIThreadFunction, NULL, 0, &threadId);
        if (hThread == NULL) {
            printf("unabel to create UI thread\n");
        }

        printf("Console ready.\n");
    }
    else if (dwReason == DLL_PROCESS_DETACH) {

        printf("detach dll\n");

        bool result = DeInitSpy(&spyData);

        printf("detach %s\n", result ? "successful" : "failed");
    }
    return TRUE;
}


signed int __stdcall hProcessEvent(UObject* object, UFunction* func, void* params) {

    // We process the event first. Sometimes we want the return result. Other times maybe not?
    int result = spyData.origProcessEvent(object, func, params);

    UClass *objClass = object->ClassPrivate;
    char *name = util::getName(objClass);
    char *funcName = util::getName(func);


    // Excludes matching class and function names.
    // if no filter, show it
    // if filter and filter matches, then skip this message.
    if (uiData.filterExcludeEnabled && uiData.filterExclude != nullptr && strlen(uiData.filterExclude) > 0) {
        char filter[2048];
        strcpy(filter, uiData.filterExclude);
        // Returns first token  
        char *token = strtok(filter, ","); 

        while (token != NULL) 
        {
            if (strstr(name, token) != NULL || strstr(funcName, token) != NULL) {
                return result;
            }
            token = strtok(NULL, ","); 
        }

    }

    // Includes matching class and function names.
    // if no filter, show all
    // if filter doesn't match, then skip this message.
    if (uiData.filterIncludeEnabled && uiData.filterInclude != nullptr && strlen(uiData.filterInclude) > 0) {
        char filter[2048];
        strcpy(filter, uiData.filterInclude);
        // Returns first token  
        char *token = strtok(filter, ","); 

        while (token != NULL) 
        {
            if (strstr(name, token) == NULL && strstr(funcName, token) == NULL) {
                return result;
            }
            token = strtok(NULL, ","); 
        }

    }



    printf("%s.%s(numParms:%d)\n", name, funcName, func->NumParms);

    // Show object properties
    if (uiData.showObjectProperties) {

        for (UProperty *p = objClass->PropertyLink; p != nullptr; p = p->PropertyLinkNext) {
            util::dumpProperty(p, object);
            printf("\n");
        }
    }

    // Show function params
    if (uiData.showFunctionParameters && params != nullptr) {

        iteratePropertiesRecursive(func, params, 0, [&](UProperty *p, void *container, int depth) {
            
            printf("%*s", depth, "");
            util::dumpProperty(p, container);
            printf("\n");

            return false;
        });

    }

	return result;
}

// Had to launch UI in its own thread.
DWORD WINAPI UIThreadFunction(LPVOID lpParam) {
    
    int result = StartUI(&uiData);
    if (result == 1) {
        printf("StartUI failed\n");
        return 1;
    }
    
    WaitForSingleObject(hThread, INFINITE);
    return 0;
}