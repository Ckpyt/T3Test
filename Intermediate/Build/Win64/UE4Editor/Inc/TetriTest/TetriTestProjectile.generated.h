// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UPrimitiveComponent;
class AActor;
struct FVector;
struct FHitResult;
#ifdef TETRITEST_TetriTestProjectile_generated_h
#error "TetriTestProjectile.generated.h already included, missing '#pragma once' in TetriTestProjectile.h"
#endif
#define TETRITEST_TetriTestProjectile_generated_h

#define TetriTest_Source_TetriTest_TetriTestProjectile_h_12_SPARSE_DATA
#define TetriTest_Source_TetriTest_TetriTestProjectile_h_12_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execOnHit);


#define TetriTest_Source_TetriTest_TetriTestProjectile_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execOnHit);


#define TetriTest_Source_TetriTest_TetriTestProjectile_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesATetriTestProjectile(); \
	friend struct Z_Construct_UClass_ATetriTestProjectile_Statics; \
public: \
	DECLARE_CLASS(ATetriTestProjectile, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/TetriTest"), NO_API) \
	DECLARE_SERIALIZER(ATetriTestProjectile) \
	static const TCHAR* StaticConfigName() {return TEXT("Game");} \



#define TetriTest_Source_TetriTest_TetriTestProjectile_h_12_INCLASS \
private: \
	static void StaticRegisterNativesATetriTestProjectile(); \
	friend struct Z_Construct_UClass_ATetriTestProjectile_Statics; \
public: \
	DECLARE_CLASS(ATetriTestProjectile, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/TetriTest"), NO_API) \
	DECLARE_SERIALIZER(ATetriTestProjectile) \
	static const TCHAR* StaticConfigName() {return TEXT("Game");} \



#define TetriTest_Source_TetriTest_TetriTestProjectile_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ATetriTestProjectile(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ATetriTestProjectile) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ATetriTestProjectile); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ATetriTestProjectile); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ATetriTestProjectile(ATetriTestProjectile&&); \
	NO_API ATetriTestProjectile(const ATetriTestProjectile&); \
public:


#define TetriTest_Source_TetriTest_TetriTestProjectile_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ATetriTestProjectile(ATetriTestProjectile&&); \
	NO_API ATetriTestProjectile(const ATetriTestProjectile&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ATetriTestProjectile); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ATetriTestProjectile); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ATetriTestProjectile)


#define TetriTest_Source_TetriTest_TetriTestProjectile_h_12_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__CollisionComp() { return STRUCT_OFFSET(ATetriTestProjectile, CollisionComp); } \
	FORCEINLINE static uint32 __PPO__ProjectileMovement() { return STRUCT_OFFSET(ATetriTestProjectile, ProjectileMovement); }


#define TetriTest_Source_TetriTest_TetriTestProjectile_h_9_PROLOG
#define TetriTest_Source_TetriTest_TetriTestProjectile_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	TetriTest_Source_TetriTest_TetriTestProjectile_h_12_PRIVATE_PROPERTY_OFFSET \
	TetriTest_Source_TetriTest_TetriTestProjectile_h_12_SPARSE_DATA \
	TetriTest_Source_TetriTest_TetriTestProjectile_h_12_RPC_WRAPPERS \
	TetriTest_Source_TetriTest_TetriTestProjectile_h_12_INCLASS \
	TetriTest_Source_TetriTest_TetriTestProjectile_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define TetriTest_Source_TetriTest_TetriTestProjectile_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	TetriTest_Source_TetriTest_TetriTestProjectile_h_12_PRIVATE_PROPERTY_OFFSET \
	TetriTest_Source_TetriTest_TetriTestProjectile_h_12_SPARSE_DATA \
	TetriTest_Source_TetriTest_TetriTestProjectile_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	TetriTest_Source_TetriTest_TetriTestProjectile_h_12_INCLASS_NO_PURE_DECLS \
	TetriTest_Source_TetriTest_TetriTestProjectile_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> TETRITEST_API UClass* StaticClass<class ATetriTestProjectile>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID TetriTest_Source_TetriTest_TetriTestProjectile_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
