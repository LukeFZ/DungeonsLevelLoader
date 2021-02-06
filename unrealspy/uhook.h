#pragma once

#include <cstdint>
#include "unrealspy.h"

namespace spy {

    struct Hook {
        uintptr_t address;
        const void *detour;       // Function to call
        void *original;    // original implementation
    };

    bool HookFunctionRef(UE4Reference refName, const void *detour, void **original);
    bool UnhookFunctionRef(UE4Reference refName);

    bool InitHook();
    bool SetHook(void *target, const void *detour, void* *original);
    // Address to target and address to our hook
    bool SetHook(Hook *hook);
    Hook *GetHook(UE4Reference refName);
    bool EnableHook(uintptr_t address);
    bool RemoveHook(Hook *hook);


}
