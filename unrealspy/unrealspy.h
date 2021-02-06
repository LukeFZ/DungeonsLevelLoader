#pragma once
#include <map>
#include <string>
#include <functional>
#include <diet-ue.h>
#include "unreal_impl.h"


#ifdef SPYAPI_IMPORT
//    #define SPYAPI __declspec(dllimport)
#else
    #define SPYAPI __declspec(dllexport)
#endif

// Would be nice if all hook stuff was declared here.
// TODO JUst move hooks array?

typedef signed int (__thiscall *tUObject_ProcessEvent) (UObject* thisObject, UFunction* func, void *params);
typedef signed int (__thiscall *tAActor_ProcessEvent) (AActor* thisActor, UFunction* func, void *params);
typedef void (__thiscall *tAHUD_PostRender) (void *hud);
typedef TNameEntryArray& (__stdcall *tFName_GetNames) ();
//typedef FRawObjectIterator *__fastcall FRawObjectIteratorCtor(FRawObjectIterator *this, bool bOnlyGCedObjects);
typedef FRawObjectIterator* (__thiscall *tFRawObjectIterator_Ctor)(void *_this, bool bOnlyGCedObjects);

// void __stdcall __high AHUD::DrawRect(struct FLinearColor, float, float, float, float)
typedef void (__thiscall *tAHUD_DrawRect)(void *hud, FLinearColor RectColor, float ScreenX, float ScreenY, float ScreenW, float ScreenH);

// void __stdcall __high AHUD::DrawText(const struct FString *, struct FLinearColor, float, float, struct UFont *, float, bool)
// 	void DrawText(const FString& Text, FLinearColor TextColor, float ScreenX, float ScreenY, UFont* Font=NULL, float Scale=1.f, bool bScalePosition=false);
typedef void (__thiscall *tAHUD_DrawText)(void *hud, const FString* Text, FLinearColor TextColor, float ScreenX, float ScreenY, UFont* Font, float Scale, bool bScalePosition);

// Sadly does not take linebreaks in to account so no good for multiline, if we care about that.
//void __fastcall AHUD::GetTextSize(AHUD *__hidden this, const struct FString *, float *, float *, struct UFont *, float)
typedef void (__thiscall *tAHUD_GetTextSize)(void *hud, const FString* Text, float* OutWidth, float* OutHeight, UFont* Font, float Scale);

//  UClass* Class, UObject* InOuter, const TCHAR* Name, const TCHAR* Filename = nullptr, uint32 LoadFlags = LOAD_None, UPackageMap* Sandbox = nullptr, bool bAllowObjectReconciliation = true, FUObjectSerializeContext* InSerializeContext = nullptr );
// _int64 __fastcall StaticLoadObject(struct UClass *a1, struct UObject *a2, const wchar_t *a3, const wchar_t *a4, unsigned int a5, struct UPackageMap *a6, bool a7, struct FUObjectSerializeContext *a8)
typedef UObject* (__fastcall *tStaticLoadObject)(UClass* Class, UObject* Inouter, const TCHAR* Name, const TCHAR* Filename, uint32 LoadFlags, void* Sandbox, bool bAllowObjectReconciliation, void *InSerializeContext);

// COREUOBJECT_API UClass* StaticLoadClass(UClass* BaseClass, UObject* InOuter, const TCHAR* Name, const TCHAR* Filename = nullptr, uint32 LoadFlags = LOAD_None, UPackageMap* Sandbox = nullptr);
typedef UClass* (__fastcall *tStaticLoadClass)(UClass* BaseClass, UObject* InOuter, const TCHAR* Name, const TCHAR* Filename, uint32 LoadFlags, void* Sandbox);

// COREUOBJECT_API UPackage* LoadPackage( UPackage* InOuter, const TCHAR* InLongPackageName, uint32 LoadFlags, FArchive* InReaderOverride = nullptr, FUObjectSerializeContext* InLoadContext = nullptr );
typedef UPackage* (__fastcall *tLoadPackage)( UPackage* InOuter, const TCHAR* InLongPackageName, uint32 LoadFlags, void* InReaderOverride, void* InLoadContext );

// COREUOBJECT_API UObject* StaticConstructObject_Internal(UClass* Class, UObject* InOuter = (UObject*)GetTransientPackage(), FName Name = NAME_None, EObjectFlags SetFlags = RF_NoFlags, EInternalObjectFlags InternalSetFlags = EInternalObjectFlags::None, UObject* Template = nullptr, bool bCopyTransientsFromClassDefaults = false, struct FObjectInstancingGraph* InstanceGraph = nullptr, bool bAssumeTemplateIsArchetype = false);
typedef UObject* (__fastcall *tStaticConstructObject_Internal)(UClass* Class, UObject* InOuter, FName Name, EObjectFlags SetFlags, EInternalObjectFlags InternalSetFlags, UObject* Template, bool bCopyTransientsFromClassDefaults, void* InstanceGraph, bool bAssumeTemplateIsArchetype);
//typedef UObject* (__fastcall *StaticConstructObject_Internal)(__int64 a1,    int a2,           __int64 a3, int a4,                int a5,                                __int64 a6,        char a7,                               __int64 a8,          char a9);

typedef void (__thiscall *tUConsole_ConsoleCommand)(UConsole *thisUConsole, const FString *Command);

// bool FConsoleManager::ProcessUserConsoleInput(const TCHAR* InInput, FOutputDevice& Ar, UWorld* InWorld)
// __int64 __fastcall FConsoleManager::ProcessUserConsoleInput(FConsoleManager *this, const wchar_t *a2, struct FOutputDevice *a3, struct UWorld *a4)
typedef void (__thiscall *tFConsoleManager_ProcessUserConsoleInput) (FConsoleManager* thisConsoleManager, const TCHAR* InInput, void *Ar, void *InWorld);

//typedef UObject* (__thiscall *tFSoftObjectPtr_LoadSynchronous) (FSoftObjectPtr *thisSoftObjectPtr);
	/**
	 * Spawn Actors with given transform and SpawnParameters
	 * 
	 * @param	Class					Class to Spawn
	 * @param	Location				Location To Spawn
	 * @param	Rotation				Rotation To Spawn
	 * @param	SpawnParameters			Spawn Parameters
	 *
	 * @return	Actor that just spawned
	 */
    // TODO UWorld* not UObject..
typedef AActor* (__thiscall *tUWorld_SpawnActor)( UObject* thisUWorld, UClass* InClass, FVector const* Location, FRotator const* Rotation, const FActorSpawnParameters& SpawnParameters );

// UUserWidget*
typedef UObject* (__fastcall *tUUserWidget_CreateWidget)(UObject *gameInstance, UClass* cls, FName name);
typedef UObject* (__thiscall *tUUserWidget_AddToViewport)(UObject *widget, int32 ZOrder);

//  UObject* InOuter, const TCHAR* Name, const TCHAR* Filename = nullptr, uint32 LoadFlags = LOAD_None, UPackageMap* Sandbox = nullptr, bool bAllowObjectReconciliation = true, FUObjectSerializeContext* InSerializeContext = nullptr );
// struct UObject *__fastcall StaticLoadObject(struct UClass *a1, struct UObject *a2, const wchar_t *a3, const wchar_t *a4, unsigned int a5, struct UPackageMap *a6, bool a7, struct FUObjectSerializeContext *a8)
//typedef UObject* (__fastcall *tStaticLoadObject)(UClass *Class, UObject *InOuter, const TCHAR *Name, const TCHAR* Filename, uint32 LoadFlags, void *Sandbox, bool bAllowObjectReconciliation, void *InSerializeContext);
// IModuleInterface* LoadModule( const FName InModuleName );
// __int64 __fastcall FModuleManager::LoadModule(__int64 a1, __int64 a2)
typedef void* (__thiscall *tFModuleManager_LoadModule)(void *thisModuleManager, FName InModuleName);

//__int64 __fastcall FModuleManager::LoadModuleWithFailureReason(__int64 a1, __int64 a2, _DWORD *a3)
// IModuleInterface* FModuleManager::LoadModuleWithFailureReason(const FName InModuleName, EModuleLoadResult& OutFailureReason)
typedef void* (__thiscall *tFModuleManager_LoadModuleWithFailureReason)(void *thisModuleManager, const FName InModuleName, uint32 &OutFailureReason);

// FModuleManager *FModuleManager::Get(void)
typedef void* (__fastcall *tFModuleManager_Get)();

// void FName::Init(const WIDECHAR* InName, int32 InNumber, EFindName FindType, bool bSplitName, int32 HardcodeIndex)
//void __stdcall __high FName::Init(const wchar_t *, int, enum EFindName, bool, int) // findType = 1, bSplitName = true, -1
//void __fastcall FName::Init(__int64 a1, wchar_t *a2, unsigned int a3, unsigned int a4, char a5, int a6)
typedef FName* (__stdcall *tFName_Init)(FName *thisFName, const wchar_t *InName, int32 InNumber, int FindType, bool bSplitName, int32 HardcodeIndex);

// void *__fastcall FWindowsPlatformProcess::GetDllHandle(const wchar_t *)
// __int64 __fastcall FWindowsPlatformProcess::GetDllHandle(const wchar_t *a1)
// void* FWindowsPlatformProcess::GetDllHandle( const TCHAR* FileName )
typedef void* (__fastcall *tFWindowsPlatformProcess_GetDllHandle)(const TCHAR *FileName);

// bool UObject::CallFunctionByNameWithArguments(const TCHAR* Str, FOutputDevice& Ar, UObject* Executor, bool bForceCallWithNonExec/*=false*/)

typedef bool (__fastcall *tUobject_CallFunctionByNameWithArguments)(UObject *thisUObject, const TCHAR* Str, FOutputDevice& Ar, UObject* Executor, bool bForceCallWithNonExec);

typedef void (__fastcall *tFStringTableRegistry_Internal_LocTableFromFile)(FName* InTableId, FString& InNamespace, FString& InFilePath, FString& InRootPath);


// We need global access to some predefined functions and names.
// Can't use string enums so maybe this?
typedef std::string UE4Reference;
const UE4Reference RefFName_GetNames                = "FName_GetNames";
const UE4Reference RefFName_Init                    = "FName::Init";
const UE4Reference RefFRawObjectIterator_Ctor       = "FRawObjectIterator_Ctor";
const UE4Reference RefUObject_ProcessEvent          = "UObject::ProcessEvent";
const UE4Reference RefAActor_ProcessEvent           = "AActor::ProcessEvent";
const UE4Reference RefAHUD_PostRender               = "AHUD::PostRender";
const UE4Reference RefStaticLoadObject              = "StaticLoadObject";
const UE4Reference RefStaticLoadClass               = "StaticLoadClass";
const UE4Reference RefLoadPackage                   = "LoadPackage";
const UE4Reference RefStaticConstructObject_Internal= "StaticConstructObject_Internal";
const UE4Reference RefUConsole_ConsoleCommand       = "UConsole_ConsoleCommand";    // Run console command
const UE4Reference RefFConsoleManager_ProcessUserConsoleInput = "FConsoleManager::ProcessUserConsoleInput";
const UE4Reference RefAddEmeralds                   = "AddEmeralds";
const UE4Reference RefLoadLevel                     = "LoadLevel";
//const UE4Reference RefCreateLevelSettings           = "CreateLevelSettings";
//const UE4Reference RefFSoftObjectPtr_LoadSynchronous= "RefFSoftObjectPtr::LoadSynchronous";
const UE4Reference RefUWorld_SpawnActor             = "UWorld_SpawnActor";
const UE4Reference RefUUserWidget_CreateWidget      = "UUserWidget::CreateWidget";
const UE4Reference RefUUserWidget_AddToViewport     = "UUserWidget::AddToViewport";
const UE4Reference RefFModuleManager_LoadModule     = "FModuleManager::LoadModule";
const UE4Reference RefFModuleManager_LoadModuleWithFailureReason = "FModuleManager::LoadModuleWithFailureReason";
const UE4Reference RefFModuleManager_Get            = "FModuleManager::Get";
const UE4Reference RefFWindowsPlatformProcess_GetDllHandle = "FWindowsPlatformProcess::GetDllHandle";
const UE4Reference RefUObject_CallFunctionByNameWithArguments = "UObject::CallFunctionByNameWithArguments";
const UE4Reference RefFStringTableRegistry_Internal_LocTableFromFile = "FStringTableRegistry::Internal_LocTableFromFile";

namespace spy {

    struct Data {
        uintptr_t baseAddress;                          // Base address of process, never used but let's leave for now.
        std::map<UE4Reference, uintptr_t> functionPtrs;  // Looked up addresses
    };

    // For internal use, but I guess I'll work that out later..
    //namespace {
    extern Data data;
    //}

    Data *Init(std::map<UE4Reference, std::string> functionPatterns);
    bool initVars();
    uintptr_t AddFunctionRef(UE4Reference refName, std::string pattern);
    uintptr_t GetFunctionRef(const UE4Reference refName);
    template<typename T>
    T GetFunction(const UE4Reference refName) {
        return (T)data.functionPtrs[refName];
    }

}