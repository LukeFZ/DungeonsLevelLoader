#include "diet-ue.h"

//#include <HAL\FMemory.inl>

// Doesn't work. We either need these compiled correctly
// or from UE4 itself.

// TODO Remove from include to see why we needed them?
// get rid of autocomplete. not worth the effort.
// Just go back to uobjects and ustructs.

void* FGenericPlatformString::Memcpy(void* Dest, const void* Src, SIZE_T Count)
{
	return memcpy(Dest, Src, Count);
	//return FMemory::Memcpy(Dest, Src, Count);
}


// //static FMalloc* GMalloc = FPlatformMemory::BaseAllocator();
// CORE_API FMalloc*			GMalloc						= nullptr;

// void* FMemory::MallocExternal(SIZE_T Count, uint32 Alignment)
// {
//     return malloc((size_t)Count);
// }

// 40 53 55 56 57 48 83 EC 38 41 8B E9 49 8B D8 48 8B FA 48 8B F1 4D 85 C0 ?? ?? ?? ?? ?? ?? ?? 33 C0 48 83 C4 38
// void* FMemory::ReallocExternal(void* Original, SIZE_T Count, uint32 Alignment)
// {
//     free(Original);
//     return malloc((size_t)Count);
// }
void *FMemory::Realloc(void* Original, SIZE_T Count, uint32 Alignment) {
	return realloc(Original, Count);
	// if (Count == 0) {
	// 	free(Original);
	// 	return nullptr;
	// }
	// void *Result;
	// Result = malloc(Count);
	// printf("Result %llx Original %llx Count %llx\n", (uintptr_t)Result, (uintptr_t)Original, Count);
	// FMemory::Memcpy(Result, Original, Count);
	// FMemory::Free(Original);
	// return Result;
}
void FMemory::Free(void *Original) {
	if (Original == nullptr) {
		return;
	}

	free(Original);
}

SIZE_T FMemory::QuantizeSize(SIZE_T Count, uint32 Alignment) {
	return Count;
}

// 40 53 55 56 57 41 56 48 83 EC 40 4C 8B F2 48 8B E9 66 85 D2 ?? ?? ?? ?? ?? ?? 48 8B DA 
// void FMemory::FreeExternal(void* Original)
// {
//     free(Original);
// }

// Not in my game.
// // If possible determine allocated size at address. Let's not.
// SIZE_T FMemory::GetAllocSizeExternal(void* Original)
// { 
//     return 0;
// }


FString FString::ToLower() const &
{
	FString New = *this;
	New.ToLowerInline();
	return New;
}

void FString::ToLowerInline()
{
	const int32 StringLength = Len();
	TCHAR* RawData = Data.GetData();
	for (int32 i = 0; i < StringLength; ++i)
	{
		RawData[i] = FChar::ToLower(RawData[i]);
	}
}

// void* FMemory::ReallocExternal(void* Original, SIZE_T Count, uint32 Alignment)
// {
//     free(Original);
//     return malloc((size_t)Count);
// }

// #pragma comment(linker, "/export:?ZeroVector@FVector@@2U1@B")
// CORE_API const FVector FVector::ZeroVector(0.0f, 0.0f, 0.0f);

#pragma comment(linker, "/export:??0FActorSpawnParameters@@QEAA@XZ")
FActorSpawnParameters::FActorSpawnParameters()
: Name(NAME_None)
, Template(NULL)
, Owner(NULL)
, Instigator(NULL)
, OverrideLevel(NULL)
, SpawnCollisionHandlingOverride(ESpawnActorCollisionHandlingMethod::Undefined)
, bRemoteOwned(false)
, bNoFail(false)
, bDeferConstruction(false)
, bAllowDuringConstructionScript(false)
#if WITH_EDITOR
, bTemporaryEditorActor(false)
#endif
, ObjectFlags(RF_Transactional)
{
}


namespace UE4Delegates_Private
{
	TAtomic<uint64> GNextID(1);
}

uint64 FDelegateHandle::GenerateNewID()
{
	// Just increment a counter to generate an ID.
	uint64 Result = ++UE4Delegates_Private::GNextID;

	// Check for the next-to-impossible event that we wrap round to 0, because we reserve 0 for null delegates.
	if (Result == 0)
	{
		// Increment it again - it might not be zero, so don't just assign it to 1.
		Result = ++UE4Delegates_Private::GNextID;
	}

	return Result;
}
