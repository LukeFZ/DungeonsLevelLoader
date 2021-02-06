#include "uhook.h"

#include <Windows/AllowWindowsPlatformTypes.h>
	#include <windows.h>
#include <Windows/HideWindowsPlatformTypes.h>

#include <MinHook.h>
#include <stdio.h>

// Enabled hooks
std::map<UE4Reference, spy::Hook*> hooks;

// We need the original set immediately or our detour
// could get called without having an original function to call
bool spy::HookFunctionRef(UE4Reference refName, const void *detour, void **original) {
    
    // Already have it
    if (hooks.count(refName) > 0) {
        return false;
    }

    Hook *hook = new Hook{GetFunctionRef(refName), detour};
    bool result = SetHook(hook);
    if (result == false) {
        return false;
    }

    // Update original if it's wanted.
    // Can still be found in hooks[]->original
    if (original != nullptr) {
        *original = hook->original;
    }

    result = EnableHook(hook->address);
    if (result == false) {
        delete hook;
        return false;
    }

    hooks[refName] = hook;

    return true;
}

bool spy::UnhookFunctionRef(UE4Reference refName) {
    
    // Don't have it
    if (hooks.count(refName) == 0) {
        return false;
    }

    Hook *hook = hooks[refName];
    bool result = RemoveHook(hook);

    // Delete regardless of removal result. Maybe?
    delete hook;
    hooks.erase(refName);

    return result;
}


bool spy::InitHook() {
    // Hook functions
    if (MH_Initialize() != MH_OK) {
        printf("MH_Initialize failed. No hooking will work\n");
        return false;
    }
    return true;
}

// Address to target and address to our hook
bool spy::SetHook(void *target, const void* detour, void* *original) {

    MH_STATUS err;

    err = MH_CreateHook(target, (LPVOID)detour, original);
    if (err != MH_OK) {
        printf("MH_CreateHook failed: %d\n", err);
        return false;
    }

    return true;
}


// Address to target and address to our hook
bool spy::SetHook(spy::Hook *hook) {

    LPVOID target = (LPVOID)(hook->address);

    return SetHook(target, hook->detour, &hook->original);
}

spy::Hook* spy::GetHook(UE4Reference refName) {
    return hooks[refName];
}

bool spy::RemoveHook(spy::Hook *hook) {
    if (hook->address == 0) {
        return false;
    }
    if (MH_DisableHook((LPVOID)hook->address) != MH_OK) {
        return false;
    }
    return true;
}

bool spy::EnableHook(uintptr_t address) {
    LPVOID target = (LPVOID)(address);
    MH_STATUS err = MH_EnableHook(target);
    if (err != MH_OK) {
        printf("MH_EnableHook failed: %d\n", err);
        return false;
    }
    return true;
}

