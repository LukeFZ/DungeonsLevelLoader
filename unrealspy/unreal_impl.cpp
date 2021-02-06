// GEngine, GUObjectArray, etc
#include "unreal_impl.h"
#include <diet-ue.h>

namespace spy {
    FUObjectArray *GUObjectArray = nullptr;
    TNameEntryArray *GNames = nullptr;
    UEngine* GEngine = nullptr;
}

//#pragma comment(linker, "/export:?ZeroVector@FVector@@2U1@B")
CORE_API const FVector FVector::ZeroVector(0.0f, 0.0f, 0.0f);