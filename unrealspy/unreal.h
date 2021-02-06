#pragma once

die not this one.
#include "tchar.h"

#include "unreal_defines.h"
#include "unreal_enums.h"
#include "unreal_draw.h"

struct UObject;
struct UClass;
struct UProperty;

//typedef void (*FNativeFuncPtr)(UObject* Context, FFrame& TheStack, RESULT_DECL);
typedef void (*FNativeFuncPtr)(UObject* Context, void * TheStack, RESULT_DECL);



/** Name index. */
typedef int32 NAME_INDEX; 	// The FNameEntry states this is a hashed value.
							// So is it hashed inside FName as well or only when added to the array table.

template<typename InElementType>//, typename InAllocator>
struct TArray {
	InElementType* Data; // A pointer to the data which we're interestd in. was void*

	int32 ArrayNum;
	int32 ArrayMax;
};

// TPair is actually TTuple in case we need that one we can change this
template<typename KeyType,typename ValueType>
struct TPair {
	KeyType key;
	ValueType value;
};

struct FString {
	/** Array holding the character data */
	//typedef TArray<TCHAR> DataType;
	TArray<TCHAR> Data;
};

struct FName // NameTypes.h
{
	NAME_INDEX Index;
	uint32_t Number;
};

#define NAME_INDEX_SHIFT 1 // one bit determines if it's a whide char or ansi char.
struct FNameEntry {
    FNameEntry* HashNext;	// For hashing purposes. It seems.
	NAME_INDEX  Index;	// Definitely hashed. NAME_INDEX_SHIFT 1. so shr 1
	
	union
	{
		char AnsiName[1024];
		char WideName[1024];
	};
};

struct FWeakObjectPtr // #WeakObjectPtr.h - used when passing UObjects, at least sometimes.
{
	int32		ObjectIndex;
	int32		ObjectSerialNumber;
};

struct FSoftObjectPath
{
	/** Asset path, patch to a top level object in a package */
	FName AssetPathName;

	/** Optional FString for subobject within an asset */
	FString SubPathString;
};

template<class TObjectID>
struct TPersistentObjectPtr
{
	// 	/** Default constructor, will be null */
	// TPersistentObjectPtr()
	// {
	// 	Reset();
	// }

	// /** Reset the lazy pointer back to the null state */
	// void Reset()
	// {
	// 	WeakPtr.Reset();
	// 	ObjectID.Reset();
	// 	TagAtLastTest = 0;
	// }

	/** Once the object has been noticed to be loaded, this is set to the object weak pointer **/
	mutable FWeakObjectPtr	WeakPtr;
	/** Compared to CurrentAnnotationTag and if they are not equal, a guid search will be performed **/
	mutable int32			TagAtLastTest;
	/** Guid for the object this pointer points to or will point to. **/
	TObjectID				ObjectID;
};
struct FSoftObjectPtr : public TPersistentObjectPtr<FSoftObjectPath>
{

};

struct FImplementedInterface {
	UClass* Class;
	/** the pointer offset of the interface's vtable */
	int32 PointerOffset;
	/** whether or not this interface has been implemented via K2 */
	bool bImplementedByK2;
};

/** A struct that maps a string name to a native function */
struct FNativeFunctionLookup {
	FName Name;
	FNativeFuncPtr Pointer;
};

struct TSharedPtr {
	//ObjectType* Object; // Actually ITextData..
	FString* Object;
//	SharedPointerInternals::FSharedReferencer< Mode > SharedReferenceCount; // Has one pointer in it
//	FReferenceControllerBase* ReferenceController;							// So just voiding it.
	void *ReferenceController;
};

// Must be incorrect.. 
struct TTextData { // implements ITextData
	void * vftable;

	//FTextDisplayStringPtr LocalizedString; // = typedef TSharedPtr<FString, ESPMode::ThreadSafe> FTextDisplayStringPtr;
    TSharedPtr LocalizedString;

	//THistoryType History; - FTextHistory has a uint16 member
                            // has a base with an FString SourceString;
};

struct TSharedRef {
	//DWORD64 unknown;
	//ObjectType* Object; // Actually ITextData..
	TTextData* Object;
//	SharedPointerInternals::FSharedReferencer< Mode > SharedReferenceCount; // Has one pointer in it
//	FReferenceControllerBase* ReferenceController;							// So just voiding it.
	void *ReferenceController;
};

enum class ESpawnActorCollisionHandlingMethod : uint8
{
	/** Fall back to default settings. */
	Undefined,
	/** Actor will spawn in desired location, regardless of collisions. */
	AlwaysSpawn,
	/** Actor will try to find a nearby non-colliding location (based on shape components), but will always spawn even if one cannot be found. */
	AdjustIfPossibleButAlwaysSpawn,
	/** Actor will try to find a nearby non-colliding location (based on shape components), but will NOT spawn unless one is found. */
	AdjustIfPossibleButDontSpawnIfColliding,
	/** Actor will fail to spawn. */
	DontSpawnIfColliding,
};

struct AActor;
struct FActorSpawnParameters
{
	//FActorSpawnParameters();

	/* A name to assign as the Name of the Actor being spawned. If no value is specified, the name of the spawned Actor will be automatically generated using the form [Class]_[Number]. */
	FName Name;

	/* An Actor to use as a template when spawning the new Actor. The spawned Actor will be initialized using the property values of the template Actor. If left NULL the class default object (CDO) will be used to initialize the spawned Actor. */
	AActor* Template;

	/* The Actor that spawned this Actor. (Can be left as NULL). */
	AActor* Owner;

	/* The APawn that is responsible for damage done by the spawned Actor. (Can be left as NULL). */
	//APawn*	Instigator;
	void* Instigator;

	/* The ULevel to spawn the Actor in, i.e. the Outer of the Actor. If left as NULL the Outer of the Owner is used. If the Owner is NULL the persistent level is used. */
	class	ULevel* OverrideLevel;

	/** Method for resolving collisions at the spawn point. Undefined means no override, use the actor's setting. */
	ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride;

private:


	/* Is the actor remotely owned. This should only be set true by the package map when it is creating an actor on a client that was replicated from the server. */
	uint16	bRemoteOwned:1;
	
public:

	bool IsRemoteOwned() const { return bRemoteOwned; }

	/* Determines whether spawning will not fail if certain conditions are not met. If true, spawning will not fail because the class being spawned is `bStatic=true` or because the class of the template Actor is not the same as the class of the Actor being spawned. */
	uint16	bNoFail:1;

	/* Determines whether the construction script will be run. If true, the construction script will not be run on the spawned Actor. Only applicable if the Actor is being spawned from a Blueprint. */
	uint16	bDeferConstruction:1;
	
	/* Determines whether or not the actor may be spawned when running a construction script. If true spawning will fail if a construction script is being run. */
	uint16	bAllowDuringConstructionScript:1;

#if WITH_EDITOR
	/** Determines whether the begin play cycle will run on the spawned actor when in the editor. */
	uint16 bTemporaryEditorActor:1;
#endif
	
	/* Flags used to describe the spawned actor/object instance. */
	EObjectFlags ObjectFlags;		
};

template <typename KeyType, typename ValueType>
class TMap {
	
};

struct FText { // size 18 bytes
/** The internal shared data for this FText */
	//TSharedRef<ITextData, ESPMode::ThreadSafe> TextData;
    TSharedRef TextData;

	/** Flags with various information on what sort of FText this is */
	uint32 Flags;
};

struct FRepRecord {
	UProperty* Property;
	int32 Index;
};


struct UObject // From UObjectBase
{
	// TODO Figure this out for a better iterator
	static const EClassCastFlags StaticClassCastFlags = CASTCLASS_None;//CLASS_Abstract|CLASS_NoExport|CLASS_Intrinsic|CLASS_MatchedSerializers;

	void *    VTableObject; // 0
	EObjectFlags  ObjectFlags; // uint32_t // 8
	/** Index into GObjectArray...very private. */
	int32_t  InternalIndex;	// 12
	/** Class the object belongs to. */
	UClass*  ClassPrivate;  // 16 // Code cmps this to see if two UProperties are the same 'type'
	/** Name of this object */
	FName    NamePrivate; 	// 24
	/** Object this object resides in. */
	UObject* OuterPrivate; // Objects with this as 0 always have flags set to nothing but 1?	// 32
};

struct UPackage : UObject {
	uint8	bDirty;
	mutable uint8 bHasBeenFullyLoaded;
	float LoadTime;
	// TODO
	// FGuid Guid;
	// TArray<int32> ChunkIDs;
	// uint32	PackageFlagsPrivate;
	// int32 PIEInstanceID;

	// FName	FileName;
	// class FLinkerLoad* LinkerLoad;
	// int32 LinkerPackageVersion;
	// int32 LinkerLicenseeVersion;
	// FCustomVersionContainer LinkerCustomVersion;
	// uint64 FileSize;
	// TUniquePtr< FWorldTileInfo > WorldTileInfo;
};

struct UField : UObject
{
	UField* Next;	// 40
};

//typedef FText(*FEnumDisplayNameFn)(int32);
struct UEnum : UField
{
	/** How this enum is declared in C++, affects the internal naming of enum values */
	enum class ECppForm
	{
		Regular,
		Namespaced,
		EnumClass
	};

	/** This will be the true type of the enum as a string, e.g. "ENamespacedEnum::InnerType" or "ERegularEnum" or "EEnumClass" */
	FString CppType;

	/** List of pairs of all enum names and values. */
	TArray<TPair<FName, int64>> Names;

	/** How the enum was originally defined. */
	ECppForm CppForm;

	/** pointer to function used to look up the enum's display name. Currently only assigned for UEnums generated for nativized blueprints */
	//FEnumDisplayNameFn EnumDisplayNameFn;
	void * EnumDisplayNameFn;

};

struct UProperty : UField
{
	// TODO Figure this out
	static const EClassCastFlags StaticClassCastFlags = CASTCLASS_None;//CLASS_Abstract|CLASS_NoExport|CLASS_Intrinsic|CLASS_MatchedSerializers;
	// Persistent variables.
	int32	ArrayDim;	// 48 - set to 1 in asm
	int32	ElementSize;	// 52
	EPropertyFlags	PropertyFlags; // 38h 56 - correct from asm
	uint16	RepIndex;	// 64

	uint8 BlueprintReplicationCondition; // Or a class pointer. // 66
										 // should be just a uint8 because it only has one property?
	int32		Offset_Internal;	// 44h 68 InOffset from constructor - correct from asm
	
	FName		RepNotifyFunc;
									// 
									// 80 is the correct one for looping. And #MovieSceneCommonHelper.h says
									// 80 should be PropertyLinkNext
	UProperty*	PropertyLinkNext;
	UProperty*  NextRef;
	UProperty*	DestructorLinkNext;
	UProperty*	PostConstructLinkNext;
	// TEnumAsByte<ELifetimeCondition> BlueprintReplicationCondition;

	// // In memory variables (generated during Link()).
	// int32		Offset_Internal;
	
	// /** In memory only: Linked list of properties from most-derived to base **/
	// UProperty*	PropertyLinkNext;
	// /** In memory only: Linked list of object reference properties from most-derived to base **/
	// UProperty*  NextRef;
	// /** In memory only: Linked list of properties requiring destruction. Note this does not include things that will be destroyed byt he native destructor **/
	// UProperty*	DestructorLinkNext;
	// /** In memory only: Linked list of properties requiring post constructor initialization.**/
	// UProperty*	PostConstructLinkNext;
};

struct UBoolProperty : UProperty {
	
	/** Size of the bitfield/bool property. Equal to ElementSize but used to check if the property has been properly initialized (0-8, where 0 means uninitialized). */
	uint8 FieldSize;
	/** Offset from the memeber variable to the byte of the property (0-7). */
	uint8 ByteOffset;
	/** Mask of the byte byte with the property value. */
	uint8 ByteMask;
	/** Mask of the field with the property value. Either equal to ByteMask or 255 in case of 'bool' type. */
	uint8 FieldMask;
	/*
		FORCEINLINE bool GetPropertyValue(void const* A) const
	{
		check(FieldSize != 0);
		uint8* ByteValue = (uint8*)A + ByteOffset;
		return !!(*ByteValue & FieldMask);
	}
		FORCEINLINE void SetPropertyValue(void* A, bool Value) const
	{
		check(FieldSize != 0);
		uint8* ByteValue = (uint8*)A + ByteOffset;
		*ByteValue = ((*ByteValue) & ~FieldMask) | (Value ? ByteMask : 0);
	}
	*/
};

struct UByteProperty : UProperty {
		// Variables.
	UEnum* Enum;
};

// Should be from UObjectProperty
struct UClassProperty : UProperty {
	class UClass* MetaClass;
};

struct UObjectPropertyBase : UProperty {
	// Variables.
	class UClass* PropertyClass;
};

struct UNumericProperty : UProperty {

};

struct UEnumProperty : UProperty {
	UNumericProperty* UnderlyingProp; // The property which represents the underlying type of the enum
	UEnum* Enum; // The enum represented by this property
};


struct UStruct : UField // class.h // 512 bytes total
{
	#if USTRUCT_FAST_ISCHILDOF_IMPL == USTRUCT_ISCHILDOF_STRUCTARRAY
//		DWORD64 StructBaseChainArray; //FStructBaseChain** StructBaseChainArray;
//		int32 NumStructBasesInChainMinusOne;
	#endif
		void * StructBaseChainArray; //FStructBaseChain** StructBaseChainArray; // 48
		int32 NumStructBasesInChainMinusOne; 									// 56 + alignment so next is 64
	
	// Sets (40) UField.Next to null in ctor

	/** Struct this inherits from, may be null */
	UStruct* SuperStruct;	// 40h 64 - SetSuperStruct uses this + 40h (this + 64)
	/** Pointer to start of linked list of child fields */
	UField*  Children;		// 48h 72 
	/** Total size of all UProperties, the allocated structure may be larger due to alignment */
	int32_t  PropertiesSize;	// 50h 80 - Must be 80, due to asm
	/** Alignment of structure in memory, structure will be at least this large */
	int32_t  MinAlignment;	// 54h 84 - Set to 1 in ctor
	
	/** Script bytecode associated with this object */
	TArray<uint8> Script;	// TArray<uint8> Script;

	UProperty* PropertyLink;	// 68h 104 - 104 is correct #MovieSceneCommonHelper.h - used to find prop by name.
	UProperty* RefLink;			// 70h 112
	UProperty* DestructorLink;
	UProperty* PostConstructLink;

	/** Array of object references embedded in script code. Mirrored for easy access by realtime garbage collection code */
	TArray<UObject*> ScriptObjectReferences; // TArray<UObject*>
};

struct UFunction;
// UObject/Class.h
struct UClass : UStruct {  // Inherhits from UStruct
	void * ClassConstructor;	// 152
	void * ClassVTableHelperCtorCaller; // 160
	void * ClassAddReferencedObjects; // 168
	//DWORD32 ClassUniqueAndBCooked; // 
	uint32 ClassUnique:31;
	uint32 bCooked:1;
//	uint32 ClassUnique; // 176 - set to 0
//	uint32 bCooked;		// 180 - InClassFlags | 0x80
	EClassFlags ClassFlags; // Also 180 !!?!?
	EClassCastFlags ClassCastFlags; // 184
	UClass* ClassWithin;	// 192
	UObject* ClassGeneratedBy;	// 200
	FName ClassConfigName;	// 208 - set to InClassConfigName

	// Other stuff here I hope we don't need to care about.

	// /** List of replication records */
	TArray<FRepRecord> ClassReps;

	// /** List of network relevant fields (properties and functions) */
	TArray<UField*> NetFields;


	/** The class default object; used for delta serialization and object initialization */
	UObject* ClassDefaultObject;

	
	// /** Map of all functions by name contained in this class */
	TMap<FName, UFunction*> FuncMap;

	// /** A cache of all functions by name that exist in a parent (superclass or interface) context */
	mutable TMap<FName, UFunction*> SuperFuncMap;

	// /** Scope lock to avoid the SuperFuncMap being read and written to simultaneously on multiple threads. */
	void *SuperFuncMapLock;

	TArray<FImplementedInterface> Interfaces;

	// /** Reference token stream used by realtime garbage collector, finalized in AssembleReferenceTokenStream */
	// FGCReferenceTokenStream ReferenceTokenStream;
	TArray<uint32>	ReferenceTokenStreamTokens;
	// /** CS for the token stream. Token stream can assemble code can sometimes be called from two threads throuh a web of async loading calls. */
	// FCriticalSection ReferenceTokenStreamCritical;
	void* Opaque1[1]; long Opaque2[2]; void* Opaque3[3];

	TArray<FNativeFunctionLookup> NativeFunctionLookupTable;
};


struct UFunction : UStruct {
	
	/** EFunctionFlags set defined for this function */
	EFunctionFlags FunctionFlags; // 98h - 152

	// Variables in memory only.
	
	/** Number of parameters total */
	uint8 NumParms; // 9c 156
	/** Total size of parameters in memory */
	uint16 ParmsSize; // 9e 158 (157 but +1 align surely)
	/** Memory offset of return value property */
	uint16 ReturnValueOffset; // 160
	/** Id of this RPC function call (must be FUNC_Net & (FUNC_NetService|FUNC_NetResponse)) */
	uint16 RPCId; // 162 - set in constructor? why?
	/** Id of the corresponding response call (must be FUNC_Net & FUNC_NetService) */
	uint16 RPCResponseId; // 164

	/** pointer to first local struct property in this UFunction that contains defaults */
	UProperty* FirstPropertyToInit; // 168 - matches constructor actually.

#if UE_BLUEPRINT_EVENTGRAPH_FASTCALLS
	/** The event graph this function calls in to (persistent) */
	UFunction* EventGraphFunction;	// 174 - 176 in ctor

	/** The state offset inside of the event graph (persistent) */
	int32 EventGraphCallOffset;		// 182 - 184 in ctor. Check our alignment

#endif
	/** C++ function this is bound to */
	FNativeFuncPtr Func;			// 186 - Should be 184 though..?

};


struct UFont : UObject {

};

struct ICppStructOps {
	/** sizeof() of the structure **/
	const int32 Size;
	/** ALIGNOF() of the structure **/
	const int32 Alignment;
};

struct UScriptStruct : UStruct {
	EStructFlags StructFlags;
	/** true if we have performed PrepareCppStructOps **/
	bool bPrepareCppStructOpsCompleted;
	/** Holds the Cpp ctors and dtors, sizeof, etc. Is not owned by this and is not released. **/
	ICppStructOps*  CppStructOps; // ICppStructOps*
};

struct UStructProperty : UProperty {
	// Variables. - What does this mean? Variable data
	class UScriptStruct* Struct; // 8 bytes

};

struct FMulticastScriptDelegate {
	//typedef TArray< TScriptDelegate<TWeakPtr> > FInvocationList;
	//FInvocationList InvocationList;		// Mutable so that we can housekeep list even with 'const' broadcasts
	TArray<void *> InvocationList; // An Array of TScriptDelegate which looks like:
	
	/** The object bound to this delegate, or nullptr if no object is bound */
	//TWeakPtr Object; // FWeakObjectPtr actually? 

	/** Name of the function to call on the bound object */
	//FName FunctionName;
	
	// FWeakObjectPtr has:
	
	//int32		ObjectIndex;
	//int32		ObjectSerialNumber;
};

struct UArrayProperty : UProperty { // inherits from TProperty<FScriptArray, UProperty>
	UProperty* Inner;
};

struct FScriptArray {
	void* Data; // FScriptContainerElement From FHeapAllocator::ForAnyElementType
	int32	  ArrayNum;	// Loop until this on Data. But increment with NumBytesPerElement.. which is?
	int32	  ArrayMax;
};

//template<typename InTCppType, class TInPropertyBaseClass>
//class TProperty : public TInPropertyBaseClass, public TPropertyTypeFundamentals<InTCppType>
//TInPropertyBaseClass = UProperty
//TPropertyTypeFundamentals basically just casts (FScriptArray*)container;

// typedef TMulticastScriptDelegate<> FMulticastScriptDelegate;

// // 

// template<typename InTCppType>
// class TPropertyTypeFundamentals
// 	/** Get the value of the property from an address */
// 	static FORCEINLINE TCppType const& GetPropertyValue(void const* A)
// 	{
// 		return *GetPropertyValuePtr(A);
// 	}

// template<typename InTCppType, class TInPropertyBaseClass>
// class TProperty : public TInPropertyBaseClass, public TPropertyTypeFundamentals<InTCppType>

// typedef TProperty<FMulticastScriptDelegate, UProperty> UMulticastDelegateProperty_Super;

// class COREUOBJECT_API UMulticastDelegateProperty : public UMulticastDelegateProperty_Super
// FMulticastScriptDelegate = TMulticastScriptDelegate - which has a:
// 		mutable FInvocationList InvocationList;		// Mutable so that we can housekeep list even with 'const' broadcasts


// So GetPropertyValue from UMulticastDelegateProperty should return a:
// FMulticastScriptDelegate. which is a TMulticastScriptDelegate with no virtual functions
struct UMulticastDelegateProperty : UProperty {
	UFunction* SignatureFunction;
};

/**
* Single item in the UObject array.
*/
struct FUObjectItem
{
	// Pointer to the allocated object
	//class UObjectBase* Object;
	UObject *Object;

	// Internal flags
	int32 Flags;
	// UObject Owner Cluster Index
	int32 ClusterRootIndex;	
	// Weak Object Pointer Serial number associated with the object
	int32 SerialNumber;

//	int32 padding;

};

struct TUObjectArray // typedef FChunkedFixedUObjectArray UObjectArray.h
{
	enum
	{
		NumElementsPerChunk = 64 * 1024,
	};

	// Objects is array of pointers to beginning of NumChunks chunks of FUObjectItems. 

	/** Master table to chunks of pointers **/
	FUObjectItem** Objects;				// 10h (16)
	/** If requested, a contiguous memory where all objects are allocated **/
	FUObjectItem* PreAllocatedObjects;	// 18h 24
	/** Maximum number of elements **/
	int32 MaxElements;					// 20h 32
	/** Number of elements we currently have **/
	int32 NumElements;					// 24h 36
	/** Maximum number of chunks **/
	int32 MaxChunks;					// 28h 40
	/** Number of chunks we currently have **/
	int32 NumChunks;					// 2ch 44
};

struct FUObjectArray
{
	int32 ObjFirstGCIndex;  			// 00h
	int32 ObjLastNonGCIndex;  			// 04h
	int32 MaxObjectsNotConsideredByGC; 	// 08h
	bool OpenForDisregardForGC;  		// boolean // 0ch (12)
	//char pad_0x000D[0x3]; //0x000D
	TUObjectArray ObjObjects; //0x0010 	// 10h (16)

};

struct FRotator
{
public:
	/** Rotation around the right axis (around Y axis), Looking up and down (0=Straight Ahead, +Up, -Down) */
	float Pitch; 

	/** Rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South. */
	float Yaw; 

	/** Rotation around the forward axis (around X axis), Tilting your head, 0=Straight, +Clockwise, -CCW. */
	float Roll;
};
struct FCommonViewportClient {
	mutable float CachedDPIScale;
	mutable bool bShouldUpdateDPIScale;
};
struct UScriptViewportClient : UObject { // Actually inherits from UObject AND FCommonViewportClient
	FCommonViewportClient temp;
};

struct UGameViewportClient : UScriptViewportClient {
};

struct UConsole : UObject {
};

struct UEngine : UObject {
	// This stuff is pretty pointless now because we can just reflection our way
	// to any UObject property, such as the GameViewport
	char unknown[0x748 - sizeof(UObject)];
//	class IEngineLoop* EngineLoop;
	void* EngineLoop;				// At 0x748
	class UGameViewportClient* GameViewport;	// At 0x750

};

struct AActor : UObject {
	// todo
};

// TODO Move to console.h
class IConsoleObject {
	public:
	virtual ~IConsoleObject() {};
	virtual const TCHAR* GetHelp() const = 0;
};

struct IConsoleCommand : IConsoleObject{
	virtual void Something();
	//virtual bool Execute( const TArray< FString >& Args, UWorld* InWorld, class FOutputDevice& OutputDevice ) = 0;
};

class FConsoleCommandBase : IConsoleCommand {
public:

	virtual const TCHAR* GetHelp() const
	{
		return nullptr;//*Help;
	}

	FString Help;

	EConsoleVariableFlags Flags;
};

// Not at all what a TMap would look like
struct ConsoleManagerObjectsMapElement {
	TArray<char*> Name;
	FConsoleCommandBase *IConsoleObject;
	uint32 HashMaybe;
	int HashSizeMaybe;
};

class FConsoleManager
{
public:
	
	
	// // internally needed or ECVF_RenderThreadSafe
	// IConsoleThreadPropagation* GetThreadPropagationCallback();
	// // internally needed or ECVF_RenderThreadSafe
	// bool IsThreadPropagationThread();

	// /** @param InVar must not be 0 */
	// FString FindConsoleObjectName(const IConsoleObject* Obj) const;

	// /** Can be moved out into some automated testing system */
	// void Test();

	// void OnCVarChanged();

	// interface IConsoleManager -----------------------------------

	// virtual IConsoleVariable* RegisterConsoleVariable(const TCHAR* Name, int32 DefaultValue, const TCHAR* Help, uint32 Flags) override;
	// virtual IConsoleVariable* RegisterConsoleVariable(const TCHAR* Name, float DefaultValue, const TCHAR* Help, uint32 Flags) override;
	// virtual IConsoleVariable* RegisterConsoleVariable(const TCHAR* Name, const FString& DefaultValue, const TCHAR* Help, uint32 Flags) override;
	// virtual IConsoleVariable* RegisterConsoleVariableRef(const TCHAR* Name, int32& RefValue, const TCHAR* Help, uint32 Flags) override;
	// virtual IConsoleVariable* RegisterConsoleVariableRef(const TCHAR* Name, float& RefValue, const TCHAR* Help, uint32 Flags) override;
	// virtual IConsoleVariable* RegisterConsoleVariableRef(const TCHAR* Name, bool& RefValue, const TCHAR* Help, uint32 Flags) override;
	// virtual IConsoleVariable* RegisterConsoleVariableBitRef(const TCHAR* CVarName, const TCHAR* FlagName, uint32 BitNumber, uint8* Force0MaskPtr, uint8* Force1MaskPtr, const TCHAR* Help, uint32 Flags) override;
	virtual void *a();
	

//private: // ----------------------------------------------------

	// Poor man's implementation of TMap - in other words: No way am I implementing all of the TMap stuff.
	// Actually TSet Pairs is the variable. TMap->Num() -> Pairs.Num() -> Elements->Num() -> return Data.Num() - NumFreeIndices;
	TArray<ConsoleManagerObjectsMapElement> ConsoleObjects;


};




template <typename ElementType, int32_t MaxTotalElements, int32_t ElementsPerChunk>
class TStaticIndirectArrayThreadSafeRead
{
	
	enum
	{
		ChunkTableSize = (MaxTotalElements + ElementsPerChunk - 1) / ElementsPerChunk
	};

	ElementType** Chunks[ChunkTableSize];
	int32       NumElements;
	int32       NumChunks;

public:
	int32 Num() const
	{
		return NumElements;
	}

	// bool IsValidIndex(const int32_t index) const
	// {
	// 	return index >= 0 && index < Num() && GetById(index) != nullptr;
	// }

	ElementType const* const& GetByIndex(int32 index) const
	{
		const ElementType *const *item = GetItemPtr(index);
		if (item == nullptr) {
			return nullptr;
		}
		return *GetItemPtr(index);
	}

private:
	ElementType const* const* GetItemPtr(int32 Index) const
	{
		//const auto ChunkIndex       = Index / ElementsPerChunk;
		//const auto WithinChunkIndex = Index % ElementsPerChunk;
		//const auto Chunk            = chunks[ChunkIndex];

		//return Chunk + WithinChunkIndex;

		int32 ChunkIndex = Index / ElementsPerChunk;
		int32 WithinChunkIndex = Index % ElementsPerChunk;
		if (Index < 0 || Index >= Num()) {
			return nullptr;
		}
		if (ChunkIndex >= NumChunks) {
			return nullptr;
		}

		if (Index >= MaxTotalElements) {
			return nullptr;
		}

		ElementType** Chunk = Chunks[ChunkIndex];
		if (Chunk == nullptr) {
			return nullptr;
		}

		return Chunk + WithinChunkIndex;

	}

};

//typedef TStaticIndirectArrayThreadSafeRead<FNameEntry, 4 * 1024 * 1024 /* 4M unique FNames */, 16384 /* allocated in 64K/128K chunks */ > TNameEntryArray;

using TNameEntryArray = TStaticIndirectArrayThreadSafeRead<FNameEntry, 4 * 1024 * 1024, 16384>;


