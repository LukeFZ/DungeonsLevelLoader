#pragma once
#include "unreal_defines.h"


/** Objects flags for internal use (GC, low level UObject code) */
enum class EInternalObjectFlags : int32
{
	None = 0,
	//~ All the other bits are reserved, DO NOT ADD NEW FLAGS HERE!

	ReachableInCluster = 1 << 23, ///< External reference to object in cluster exists
	ClusterRoot = 1 << 24, ///< Root of a cluster
	Native = 1 << 25, ///< Native (UClass only). 
	Async = 1 << 26, ///< Object exists only on a different thread than the game thread.
	AsyncLoading = 1 << 27, ///< Object is being asynchronously loaded.
	Unreachable = 1 << 28, ///< Object is not reachable on the object graph.
	PendingKill = 1 << 29, ///< Objects that are pending destruction (invalid for gameplay but valid objects)
	RootSet = 1 << 30, ///< Object will not be garbage collected, even if unreferenced.
	//~ UnusedFlag = 1 << 31,

	GarbageCollectionKeepFlags = Native | Async | AsyncLoading,

	//~ Make sure this is up to date!
	AllFlags = ReachableInCluster | ClusterRoot | Native | Async | AsyncLoading | Unreachable | PendingKill | RootSet
};


/**
 * Flags associated with each property in a class, overriding the
 * property's default behavior.
 * @warning When adding one here, please update ParsePropertyFlags()
 */
enum EPropertyFlags : uint64
{
	CPF_None = 0,

	CPF_Edit							= 0x0000000000000001,	///< Property is user-settable in the editor.
	CPF_ConstParm						= 0x0000000000000002,	///< This is a constant function parameter
	CPF_BlueprintVisible				= 0x0000000000000004,	///< This property can be read by blueprint code
	CPF_ExportObject					= 0x0000000000000008,	///< Object can be exported with actor.
	CPF_BlueprintReadOnly				= 0x0000000000000010,	///< This property cannot be modified by blueprint code
	CPF_Net								= 0x0000000000000020,	///< Property is relevant to network replication.
	CPF_EditFixedSize					= 0x0000000000000040,	///< Indicates that elements of an array can be modified, but its size cannot be changed.
	CPF_Parm							= 0x0000000000000080,	///< Function/When call parameter.
	CPF_OutParm							= 0x0000000000000100,	///< Value is copied out after function call.
	CPF_ZeroConstructor					= 0x0000000000000200,	///< memset is fine for construction
	CPF_ReturnParm						= 0x0000000000000400,	///< Return value.
	CPF_DisableEditOnTemplate			= 0x0000000000000800,	///< Disable editing of this property on an archetype/sub-blueprint
	//CPF_      						= 0x0000000000001000,	///< 
	CPF_Transient   					= 0x0000000000002000,	///< Property is transient: shouldn't be saved or loaded, except for Blueprint CDOs.
	CPF_Config      					= 0x0000000000004000,	///< Property should be loaded/saved as permanent profile.
	//CPF_								= 0x0000000000008000,	///< 
	CPF_DisableEditOnInstance			= 0x0000000000010000,	///< Disable editing on an instance of this class
	CPF_EditConst   					= 0x0000000000020000,	///< Property is uneditable in the editor.
	CPF_GlobalConfig					= 0x0000000000040000,	///< Load config from base class, not subclass.
	CPF_InstancedReference				= 0x0000000000080000,	///< Property is a component references.
	//CPF_								= 0x0000000000100000,	///<
	CPF_DuplicateTransient				= 0x0000000000200000,	///< Property should always be reset to the default value during any type of duplication (copy/paste, binary duplication, etc.)
	CPF_SubobjectReference				= 0x0000000000400000,	///< Property contains subobject references (TSubobjectPtr)
	//CPF_    							= 0x0000000000800000,	///< 
	CPF_SaveGame						= 0x0000000001000000,	///< Property should be serialized for save games
	CPF_NoClear							= 0x0000000002000000,	///< Hide clear (and browse) button.
	//CPF_  							= 0x0000000004000000,	///<
	CPF_ReferenceParm					= 0x0000000008000000,	///< Value is passed by reference; CPF_OutParam and CPF_Param should also be set.
	CPF_BlueprintAssignable				= 0x0000000010000000,	///< MC Delegates only.  Property should be exposed for assigning in blueprint code
	CPF_Deprecated  					= 0x0000000020000000,	///< Property is deprecated.  Read it from an archive, but don't save it.
	CPF_IsPlainOldData					= 0x0000000040000000,	///< If this is set, then the property can be memcopied instead of CopyCompleteValue / CopySingleValue
	CPF_RepSkip							= 0x0000000080000000,	///< Not replicated. For non replicated properties in replicated structs 
	CPF_RepNotify						= 0x0000000100000000,	///< Notify actors when a property is replicated
	CPF_Interp							= 0x0000000200000000,	///< interpolatable property for use with matinee
	CPF_NonTransactional				= 0x0000000400000000,	///< Property isn't transacted
	CPF_EditorOnly						= 0x0000000800000000,	///< Property should only be loaded in the editor
	CPF_NoDestructor					= 0x0000001000000000,	///< No destructor
	//CPF_								= 0x0000002000000000,	///<
	CPF_AutoWeak						= 0x0000004000000000,	///< Only used for weak pointers, means the export type is autoweak
	CPF_ContainsInstancedReference		= 0x0000008000000000,	///< Property contains component references.
	CPF_AssetRegistrySearchable			= 0x0000010000000000,	///< asset instances will add properties with this flag to the asset registry automatically
	CPF_SimpleDisplay					= 0x0000020000000000,	///< The property is visible by default in the editor details view
	CPF_AdvancedDisplay					= 0x0000040000000000,	///< The property is advanced and not visible by default in the editor details view
	CPF_Protected						= 0x0000080000000000,	///< property is protected from the perspective of script
	CPF_BlueprintCallable				= 0x0000100000000000,	///< MC Delegates only.  Property should be exposed for calling in blueprint code
	CPF_BlueprintAuthorityOnly			= 0x0000200000000000,	///< MC Delegates only.  This delegate accepts (only in blueprint) only events with BlueprintAuthorityOnly.
	CPF_TextExportTransient				= 0x0000400000000000,	///< Property shouldn't be exported to text format (e.g. copy/paste)
	CPF_NonPIEDuplicateTransient		= 0x0000800000000000,	///< Property should only be copied in PIE
	CPF_ExposeOnSpawn					= 0x0001000000000000,	///< Property is exposed on spawn
	CPF_PersistentInstance				= 0x0002000000000000,	///< A object referenced by the property is duplicated like a component. (Each actor should have an own instance.)
	CPF_UObjectWrapper					= 0x0004000000000000,	///< Property was parsed as a wrapper class like TSubclassOf<T>, FScriptInterface etc., rather than a USomething*
	CPF_HasGetValueTypeHash				= 0x0008000000000000,	///< This property can generate a meaningful hash value.
	CPF_NativeAccessSpecifierPublic		= 0x0010000000000000,	///< Public native access specifier
	CPF_NativeAccessSpecifierProtected	= 0x0020000000000000,	///< Protected native access specifier
	CPF_NativeAccessSpecifierPrivate	= 0x0040000000000000,	///< Private native access specifier
	CPF_SkipSerialization				= 0x0080000000000000,	///< Property shouldn't be serialized, can still be exported to text
};

/**
 * Flags describing a class.
 */
enum EClassFlags // from UObject/ObjectMacros.h
{
	/** No Flags */
	CLASS_None				  = 0x00000000u,
	/** Class is abstract and can't be instantiated directly. */
	CLASS_Abstract            = 0x00000001u,
	/** Save object configuration only to Default INIs, never to local INIs. Must be combined with CLASS_Config */
	CLASS_DefaultConfig		  = 0x00000002u,
	/** Load object configuration at construction time. */
	CLASS_Config			  = 0x00000004u,
	/** This object type can't be saved; null it out at save time. */
	CLASS_Transient			  = 0x00000008u,
	/** Successfully parsed. */
	CLASS_Parsed              = 0x00000010u,
	/** */
	CLASS_MatchedSerializers  = 0x00000020u,
	/** All the properties on the class are shown in the advanced section (which is hidden by default) unless SimpleDisplay is specified on the property */
	CLASS_AdvancedDisplay	  = 0x00000040u,
	/** Class is a native class - native interfaces will have CLASS_Native set, but not RF_MarkAsNative */
	CLASS_Native			  = 0x00000080u,
	/** Don't export to C++ header. */
	CLASS_NoExport            = 0x00000100u,
	/** Do not allow users to create in the editor. */
	CLASS_NotPlaceable        = 0x00000200u,
	/** Handle object configuration on a per-object basis, rather than per-class. */
	CLASS_PerObjectConfig     = 0x00000400u,
	
	/** Whether SetUpRuntimeReplicationData still needs to be called for this class */
	CLASS_ReplicationDataIsSetUp = 0x00000800u,
	
	/** Class can be constructed from editinline New button. */
	CLASS_EditInlineNew		  = 0x00001000u,
	/** Display properties in the editor without using categories. */
	CLASS_CollapseCategories  = 0x00002000u,
	/** Class is an interface **/
	CLASS_Interface           = 0x00004000u,
	/**  Do not export a constructor for this class, assuming it is in the cpptext **/
	CLASS_CustomConstructor   = 0x00008000u,
	/** all properties and functions in this class are const and should be exported as const */
	CLASS_Const			      = 0x00010000u,

	/** Class flag indicating the class is having its layout changed, and therefore is not ready for a CDO to be created */
	CLASS_LayoutChanging	  = 0x00020000u,
	
	/** Indicates that the class was created from blueprint source material */
	CLASS_CompiledFromBlueprint  = 0x00040000u,

	/** Indicates that only the bare minimum bits of this class should be DLL exported/imported */
	CLASS_MinimalAPI	      = 0x00080000u,
	
	/** Indicates this class must be DLL exported/imported (along with all of it's members) */
	CLASS_RequiredAPI	      = 0x00100000u,

	/** Indicates that references to this class default to instanced. Used to be subclasses of UComponent, but now can be any UObject */
	CLASS_DefaultToInstanced  = 0x00200000u,

	/** Indicates that the parent token stream has been merged with ours. */
	CLASS_TokenStreamAssembled  = 0x00400000u,
	/** Class has component properties. */
	CLASS_HasInstancedReference= 0x00800000u,
	/** Don't show this class in the editor class browser or edit inline new menus. */
	CLASS_Hidden			  = 0x01000000u,
	/** Don't save objects of this class when serializing */
	CLASS_Deprecated		  = 0x02000000u,
	/** Class not shown in editor drop down for class selection */
	CLASS_HideDropDown		  = 0x04000000u,
	/** Class settings are saved to <AppData>/..../Blah.ini (as opposed to CLASS_DefaultConfig) */
	CLASS_GlobalUserConfig	  = 0x08000000u,
	/** Class was declared directly in C++ and has no boilerplate generated by UnrealHeaderTool */
	CLASS_Intrinsic			  = 0x10000000u,
	/** Class has already been constructed (maybe in a previous DLL version before hot-reload). */
	CLASS_Constructed		  = 0x20000000u,
	/** Indicates that object configuration will not check against ini base/defaults when serialized */
	CLASS_ConfigDoNotCheckDefaults = 0x40000000u,
	/** Class has been consigned to oblivion as part of a blueprint recompile, and a newer version currently exists. */
	CLASS_NewerVersionExists  = 0x80000000u,
};


/**
 * Flags used for quickly casting classes of certain types; all class cast flags are inherited
 */
enum EClassCastFlags : uint64  // from UObject/ObjectMacros.h
{
	CASTCLASS_None = 0x0000000000000000,

	CASTCLASS_UField						= 0x0000000000000001,
	CASTCLASS_UInt8Property					= 0x0000000000000002,
	CASTCLASS_UEnum							= 0x0000000000000004,
	CASTCLASS_UStruct						= 0x0000000000000008,
	CASTCLASS_UScriptStruct					= 0x0000000000000010,
	CASTCLASS_UClass						= 0x0000000000000020,
	CASTCLASS_UByteProperty					= 0x0000000000000040,
	CASTCLASS_UIntProperty					= 0x0000000000000080,
	CASTCLASS_UFloatProperty				= 0x0000000000000100,
	CASTCLASS_UUInt64Property				= 0x0000000000000200,
	CASTCLASS_UClassProperty				= 0x0000000000000400,
	CASTCLASS_UUInt32Property				= 0x0000000000000800,
	CASTCLASS_UInterfaceProperty			= 0x0000000000001000,
	CASTCLASS_UNameProperty					= 0x0000000000002000,
	CASTCLASS_UStrProperty					= 0x0000000000004000,
	CASTCLASS_UProperty						= 0x0000000000008000,
	CASTCLASS_UObjectProperty				= 0x0000000000010000,
	CASTCLASS_UBoolProperty					= 0x0000000000020000,
	CASTCLASS_UUInt16Property				= 0x0000000000040000,
	CASTCLASS_UFunction						= 0x0000000000080000,
	CASTCLASS_UStructProperty				= 0x0000000000100000,
	CASTCLASS_UArrayProperty				= 0x0000000000200000,
	CASTCLASS_UInt64Property				= 0x0000000000400000,
	CASTCLASS_UDelegateProperty				= 0x0000000000800000,
	CASTCLASS_UNumericProperty				= 0x0000000001000000,
	CASTCLASS_UMulticastDelegateProperty	= 0x0000000002000000,
	CASTCLASS_UObjectPropertyBase			= 0x0000000004000000,
	CASTCLASS_UWeakObjectProperty			= 0x0000000008000000,
	CASTCLASS_ULazyObjectProperty			= 0x0000000010000000,
	CASTCLASS_USoftObjectProperty			= 0x0000000020000000,
	CASTCLASS_UTextProperty					= 0x0000000040000000,
	CASTCLASS_UInt16Property				= 0x0000000080000000,
	CASTCLASS_UDoubleProperty				= 0x0000000100000000,
	CASTCLASS_USoftClassProperty			= 0x0000000200000000,
	CASTCLASS_UPackage						= 0x0000000400000000,
	CASTCLASS_ULevel						= 0x0000000800000000,
	CASTCLASS_AActor						= 0x0000001000000000,
	CASTCLASS_APlayerController				= 0x0000002000000000,
	CASTCLASS_APawn							= 0x0000004000000000,
	CASTCLASS_USceneComponent				= 0x0000008000000000,
	CASTCLASS_UPrimitiveComponent			= 0x0000010000000000,
	CASTCLASS_USkinnedMeshComponent			= 0x0000020000000000,
	CASTCLASS_USkeletalMeshComponent		= 0x0000040000000000,
	CASTCLASS_UBlueprint					= 0x0000080000000000,
	CASTCLASS_UDelegateFunction				= 0x0000100000000000,
	CASTCLASS_UStaticMeshComponent			= 0x0000200000000000,
	CASTCLASS_UMapProperty					= 0x0000400000000000,
	CASTCLASS_USetProperty					= 0x0000800000000000,
	CASTCLASS_UEnumProperty					= 0x0001000000000000,
};


//
// Function flags.
//
// Note: Please keep ParseFunctionFlags in sync when this enum is modified.
enum EFunctionFlags : uint32
{
	// Function flags.
	FUNC_None				= 0x00000000,

	FUNC_Final				= 0x00000001,	// Function is final (prebindable, non-overridable function).
	FUNC_RequiredAPI			= 0x00000002,	// Indicates this function is DLL exported/imported.
	FUNC_BlueprintAuthorityOnly= 0x00000004,   // Function will only run if the object has network authority
	FUNC_BlueprintCosmetic	= 0x00000008,   // Function is cosmetic in nature and should not be invoked on dedicated servers
	// FUNC_				= 0x00000010,   // unused.
	// FUNC_				= 0x00000020,   // unused.
	FUNC_Net				= 0x00000040,   // Function is network-replicated.
	FUNC_NetReliable		= 0x00000080,   // Function should be sent reliably on the network.
	FUNC_NetRequest			= 0x00000100,	// Function is sent to a net service
	FUNC_Exec				= 0x00000200,	// Executable from command line.
	FUNC_Native				= 0x00000400,	// Native function.
	FUNC_Event				= 0x00000800,   // Event function.
	FUNC_NetResponse		= 0x00001000,   // Function response from a net service
	FUNC_Static				= 0x00002000,   // Static function.
	FUNC_NetMulticast		= 0x00004000,	// Function is networked multicast Server -> All Clients
	FUNC_UbergraphFunction	= 0x00008000,   // Function is used as the merge 'ubergraph' for a blueprint, only assigned when using the persistent 'ubergraph' frame
	FUNC_MulticastDelegate	= 0x00010000,	// Function is a multi-cast delegate signature (also requires FUNC_Delegate to be set!)
	FUNC_Public				= 0x00020000,	// Function is accessible in all classes (if overridden, parameters must remain unchanged).
	FUNC_Private			= 0x00040000,	// Function is accessible only in the class it is defined in (cannot be overridden, but function name may be reused in subclasses.  IOW: if overridden, parameters don't need to match, and Super.Func() cannot be accessed since it's private.)
	FUNC_Protected			= 0x00080000,	// Function is accessible only in the class it is defined in and subclasses (if overridden, parameters much remain unchanged).
	FUNC_Delegate			= 0x00100000,	// Function is delegate signature (either single-cast or multi-cast, depending on whether FUNC_MulticastDelegate is set.)
	FUNC_NetServer			= 0x00200000,	// Function is executed on servers (set by replication code if passes check)
	FUNC_HasOutParms		= 0x00400000,	// function has out (pass by reference) parameters
	FUNC_HasDefaults		= 0x00800000,	// function has structs that contain defaults
	FUNC_NetClient			= 0x01000000,	// function is executed on clients
	FUNC_DLLImport			= 0x02000000,	// function is imported from a DLL
	FUNC_BlueprintCallable	= 0x04000000,	// function can be called from blueprint code
	FUNC_BlueprintEvent		= 0x08000000,	// function can be overridden/implemented from a blueprint
	FUNC_BlueprintPure		= 0x10000000,	// function can be called from blueprint code, and is also pure (produces no side effects). If you set this, you should set FUNC_BlueprintCallable as well.
	FUNC_EditorOnly			= 0x20000000,	// function can only be called from an editor scrippt.
	FUNC_Const				= 0x40000000,	// function can be called from blueprint code, and only reads state (never writes state)
	FUNC_NetValidate		= 0x80000000,	// function must supply a _Validate implementation

	FUNC_AllFlags		= 0xFFFFFFFF,
};

/** 
 * Flags describing an object instance
 */
enum EObjectFlags  // from ObjectMacros.h
{
	// Do not add new flags unless they truly belong here. There are alternatives.
	// if you change any the bit of any of the RF_Load flags, then you will need legacy serialization
	RF_NoFlags					= 0x00000000,	///< No flags, used to avoid a cast

	// This first group of flags mostly has to do with what kind of object it is. Other than transient, these are the persistent object flags.
	// The garbage collector also tends to look at these.
	RF_Public					=0x00000001,	///< Object is visible outside its package.
	RF_Standalone				=0x00000002,	///< Keep object around for editing even if unreferenced.
	RF_MarkAsNative				=0x00000004,	///< Object (UField) will be marked as native on construction (DO NOT USE THIS FLAG in HasAnyFlags() etc)
	RF_Transactional			=0x00000008,	///< Object is transactional.
	RF_ClassDefaultObject		=0x00000010,	///< This object is its class's default object
	RF_ArchetypeObject			=0x00000020,	///< This object is a template for another object - treat like a class default object
	RF_Transient				=0x00000040,	///< Don't save object.

	// This group of flags is primarily concerned with garbage collection.
	RF_MarkAsRootSet			=0x00000080,	///< Object will be marked as root set on construction and not be garbage collected, even if unreferenced (DO NOT USE THIS FLAG in HasAnyFlags() etc)
	RF_TagGarbageTemp			=0x00000100,	///< This is a temp user flag for various utilities that need to use the garbage collector. The garbage collector itself does not interpret it.

	// The group of flags tracks the stages of the lifetime of a uobject
	RF_NeedInitialization		=0x00000200,	///< This object has not completed its initialization process. Cleared when ~FObjectInitializer completes
	RF_NeedLoad					=0x00000400,	///< During load, indicates object needs loading.
	RF_KeepForCooker			=0x00000800,	///< Keep this object during garbage collection because it's still being used by the cooker
	RF_NeedPostLoad				=0x00001000,	///< Object needs to be postloaded.
	RF_NeedPostLoadSubobjects	=0x00002000,	///< During load, indicates that the object still needs to instance subobjects and fixup serialized component references
	RF_NewerVersionExists		=0x00004000,	///< Object has been consigned to oblivion due to its owner package being reloaded, and a newer version currently exists
	RF_BeginDestroyed			=0x00008000,	///< BeginDestroy has been called on the object.
	RF_FinishDestroyed			=0x00010000,	///< FinishDestroy has been called on the object.

	// Misc. Flags
	RF_BeingRegenerated			=0x00020000,	///< Flagged on UObjects that are used to create UClasses (e.g. Blueprints) while they are regenerating their UClass on load (See FLinkerLoad::CreateExport())
	RF_DefaultSubObject			=0x00040000,	///< Flagged on subobjects that are defaults
	RF_WasLoaded				=0x00080000,	///< Flagged on UObjects that were loaded
	RF_TextExportTransient		=0x00100000,	///< Do not export object to text form (e.g. copy/paste). Generally used for sub-objects that can be regenerated from data in their parent object.
	RF_LoadCompleted			=0x00200000,	///< Object has been completely serialized by linkerload at least once. DO NOT USE THIS FLAG, It should be replaced with RF_WasLoaded.
	RF_InheritableComponentTemplate = 0x00400000, ///< Archetype of the object can be in its super class
	RF_DuplicateTransient		=0x00800000,	///< Object should not be included in any type of duplication (copy/paste, binary duplication, etc.)
	RF_StrongRefOnFrame			=0x01000000,	///< References to this object from persistent function frame are handled as strong ones.
	RF_NonPIEDuplicateTransient	=0x02000000,	///< Object should not be included for duplication unless it's being duplicated for a PIE session
	RF_Dynamic					=0x04000000,	///< Field Only. Dynamic field - doesn't get constructed during static initialization, can be constructed multiple times
	RF_WillBeLoaded				=0x08000000,	///< This object was constructed during load and will be loaded shortly
};

/** Mask for all object flags */
#define RF_AllFlags				(EObjectFlags)0x0fffffff	///< All flags, used mainly for error checking


enum EStructFlags
{
	// State flags.
	STRUCT_NoFlags				= 0x00000000,	
	STRUCT_Native				= 0x00000001,

	/** If set, this struct will be compared using native code */
	STRUCT_IdenticalNative		= 0x00000002,
	
	STRUCT_HasInstancedReference= 0x00000004,

	STRUCT_NoExport				= 0x00000008,

	/** Indicates that this struct should always be serialized as a single unit */
	STRUCT_Atomic				= 0x00000010,

	/** Indicates that this struct uses binary serialization; it is unsafe to add/remove members from this struct without incrementing the package version */
	STRUCT_Immutable			= 0x00000020,

	/** If set, native code needs to be run to find referenced objects */
	STRUCT_AddStructReferencedObjects = 0x00000040,

	/** Indicates that this struct should be exportable/importable at the DLL layer.  Base structs must also be exportable for this to work. */
	STRUCT_RequiredAPI			= 0x00000200,	

	/** If set, this struct will be serialized using the CPP net serializer */
	STRUCT_NetSerializeNative	= 0x00000400,	

	/** If set, this struct will be serialized using the CPP serializer */
	STRUCT_SerializeNative		= 0x00000800,	

	/** If set, this struct will be copied using the CPP operator= */
	STRUCT_CopyNative			= 0x00001000,	

	/** If set, this struct will be copied using memcpy */
	STRUCT_IsPlainOldData		= 0x00002000,	

	/** If set, this struct has no destructor and non will be called. STRUCT_IsPlainOldData implies STRUCT_NoDestructor */
	STRUCT_NoDestructor			= 0x00004000,	

	/** If set, this struct will not be constructed because it is assumed that memory is zero before construction. */
	STRUCT_ZeroConstructor		= 0x00008000,	

	/** If set, native code will be used to export text */
	STRUCT_ExportTextItemNative	= 0x00010000,	

	/** If set, native code will be used to export text */
	STRUCT_ImportTextItemNative	= 0x00020000,	

	/** If set, this struct will have PostSerialize called on it after CPP serializer or tagged property serialization is complete */
	STRUCT_PostSerializeNative  = 0x00040000,

	/** If set, this struct will have SerializeFromMismatchedTag called on it if a mismatched tag is encountered. */
	STRUCT_SerializeFromMismatchedTag = 0x00080000,

	/** If set, this struct will be serialized using the CPP net delta serializer */
	STRUCT_NetDeltaSerializeNative = 0x00100000,

	/** If set, this struct will be have PostScriptConstruct called on it after a temporary object is constructed in a running blueprint */
	STRUCT_PostScriptConstruct     = 0x00200000,

	/** If set, this struct can share net serialization state across connections */
	STRUCT_NetSharedSerialization = 0x00400000,

	/**  */
	STRUCT_SerializeNativeStructured = 0x00800000,

	/** Struct flags that are automatically inherited */
	STRUCT_Inherit				= STRUCT_HasInstancedReference|STRUCT_Atomic,

	/** Flags that are always computed, never loaded or done with code generation */
	STRUCT_ComputedFlags		= STRUCT_NetDeltaSerializeNative | STRUCT_NetSerializeNative | STRUCT_SerializeNative | STRUCT_SerializeNativeStructured | STRUCT_PostSerializeNative | STRUCT_CopyNative | STRUCT_IsPlainOldData | STRUCT_NoDestructor | STRUCT_ZeroConstructor | STRUCT_IdenticalNative | STRUCT_AddStructReferencedObjects | STRUCT_ExportTextItemNative | STRUCT_ImportTextItemNative | STRUCT_SerializeFromMismatchedTag | STRUCT_PostScriptConstruct | STRUCT_NetSharedSerialization
};

enum EConsoleVariableFlags
{
	/**
	 * Default, no flags are set, the value is set by the constructor 
	 */
	ECVF_Default = 0x0,
	/**
	 * Console variables marked with this flag behave differently in a final release build.
	 * Then they are are hidden in the console and cannot be changed by the user.
	 */
	ECVF_Cheat = 0x1,
	/**
	 * Console variables cannot be changed by the user (from console).
	 * Changing from C++ or ini is still possible.
	 */
	ECVF_ReadOnly = 0x4,
	/**
	 * UnregisterConsoleObject() was called on this one.
	 * If the variable is registered again with the same type this object is reactivated. This is good for DLL unloading.
	 */
	ECVF_Unregistered = 0x8,
	/**
	 * This flag is set by the ini loading code when the variable wasn't registered yet.
	 * Once the variable is registered later the value is copied over and the variable is destructed.
	 */
	ECVF_CreatedFromIni = 0x10,
	/**
	 * Maintains another shadow copy and updates the copy with render thread commands to maintain proper ordering.
	 * Could be extended for more/other thread.
 	 * Note: On console variable references it assumes the reference is accessed on the render thread only
	 * (Don't use in any other thread or better don't use references to avoid the potential pitfall).
	 */
	ECVF_RenderThreadSafe = 0x20,

	/* ApplyCVarSettingsGroupFromIni will complain if this wasn't set, should not be combined with ECVF_Cheat */
	ECVF_Scalability = 0x40,

	/* those cvars control other cvars with the flag ECVF_Scalability, names should start with "sg." */
	ECVF_ScalabilityGroup = 0x80,

	// ------------------------------------------------

	/* to get some history of where the last value was set by ( useful for track down why a cvar is in a specific state */
	ECVF_SetByMask =				0xff000000,
	
	// the ECVF_SetBy are sorted in override order (weak to strong), the value is not serialized, it only affects it's override behavior when calling Set()

	// lowest priority (default after console variable creation)
	ECVF_SetByConstructor =			0x00000000,
	// from Scalability.ini (lower priority than game settings so it's easier to override partially)
	ECVF_SetByScalability =			0x01000000,
	// (in game UI or from file)
	ECVF_SetByGameSetting =			0x02000000,
	// project settings (editor UI or from file, higher priority than game setting to allow to enforce some setting fro this project)
	ECVF_SetByProjectSetting =		0x03000000,
	// per project setting (ini file e.g. Engine.ini or Game.ini)
	ECVF_SetBySystemSettingsIni =	0x04000000,
	// per device setting (e.g. specific iOS device, higher priority than per project to do device specific settings)
	ECVF_SetByDeviceProfile =		0x05000000,
	// consolevariables.ini (for multiple projects)
	ECVF_SetByConsoleVariablesIni = 0x06000000,
	// a minus command e.g. -VSync (very high priority to enforce the setting for the application)
	ECVF_SetByCommandline =			0x07000000,
	// least useful, likely a hack, maybe better to find the correct SetBy...
	ECVF_SetByCode =				0x08000000,
	// editor UI or console in game or editor
	ECVF_SetByConsole =				0x09000000,

	// ------------------------------------------------
};