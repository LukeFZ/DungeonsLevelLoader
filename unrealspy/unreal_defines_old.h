
#pragma once
#include <iostream>
#define uint8 uint8_t
#define int32 int32_t
#define int16 int16_t
#define int64 int64_t
#define uint8 uint8_t
#define uint16 uint16_t
#define uint32 uint32_t
#define uint64 uint64_t


#define MIN_uint8		((uint8)	0x00)
#define	MIN_uint16		((uint16)	0x0000)
#define	MIN_uint32		((uint32)	0x00000000)
#define MIN_uint64		((uint64)	0x0000000000000000)
#define MIN_int8		((int8)		-128)
#define MIN_int16		((int16)	-32768)
#define MIN_int32		((int32)	0x80000000)
#define MIN_int64		((int64)	0x8000000000000000)

#define MAX_uint8		((uint8)	0xff)
#define MAX_uint16		((uint16)	0xffff)
#define MAX_uint32		((uint32)	0xffffffff)
#define MAX_uint64		((uint64)	0xffffffffffffffff)
#define MAX_int8		((int8)		0x7f)
#define MAX_int16		((int16)	0x7fff)
#define MAX_int32		((int32)	0x7fffffff)
#define MAX_int64		((int64)	0x7fffffffffffffff)

#define MIN_flt			(1.175494351e-38F)			/* min positive value */
#define MAX_flt			(3.402823466e+38F)
#define MIN_dbl			(2.2250738585072014e-308)	/* min positive value */
#define MAX_dbl			(1.7976931348623158e+308)	

#define UE_BLUEPRINT_EVENTGRAPH_FASTCALLS 1

#define RESULT_PARAM Z_Param__Result
#define RESULT_DECL void*const RESULT_PARAM


// TODO Move this, but it's interesting stuff for what we're doing
/** The type of a native function callable by script */
//typedef void (*FNativeFuncPtr)(UObject* Context, FFrame& TheStack, RESULT_DECL);

// // This class is deliberately simple (i.e. POD) to keep generated code size down.
// struct FNameNativePtrPair
// {
// 	const char* NameUTF8;
// 	FNativeFuncPtr Pointer;
// };

// extern COREUOBJECT_API FNativeFuncPtr GCasts[];
// uint8 COREUOBJECT_API GRegisterCast( int32 CastCode, const FNativeFuncPtr& Func );


// /** A struct that maps a string name to a native function */
// struct FNativeFunctionRegistrar
// {
// 	FNativeFunctionRegistrar(class UClass* Class, const ANSICHAR* InName, FNativeFuncPtr InPointer)
// 	{
// 		RegisterFunction(Class, InName, InPointer);
// 	}
// 	static COREUOBJECT_API void RegisterFunction(class UClass* Class, const ANSICHAR* InName, FNativeFuncPtr InPointer);
// 	// overload for types generated from blueprints, which can have unicode names:
// 	static COREUOBJECT_API void RegisterFunction(class UClass* Class, const WIDECHAR* InName, FNativeFuncPtr InPointer);

// 	static COREUOBJECT_API void RegisterFunctions(class UClass* Class, const FNameNativePtrPair* InArray, int32 NumFunctions);
// };
