// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef TETRITEST_TetriTestCharacter_generated_h
#error "TetriTestCharacter.generated.h already included, missing '#pragma once' in TetriTestCharacter.h"
#endif
#define TETRITEST_TetriTestCharacter_generated_h

#define TetriTest_Source_TetriTest_TetriTestCharacter_h_26_SPARSE_DATA
#define TetriTest_Source_TetriTest_TetriTestCharacter_h_26_RPC_WRAPPERS
#define TetriTest_Source_TetriTest_TetriTestCharacter_h_26_RPC_WRAPPERS_NO_PURE_DECLS
#define TetriTest_Source_TetriTest_TetriTestCharacter_h_26_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesATetriTestCharacter(); \
	friend struct Z_Construct_UClass_ATetriTestCharacter_Statics; \
public: \
	DECLARE_CLASS(ATetriTestCharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/TetriTest"), NO_API) \
	DECLARE_SERIALIZER(ATetriTestCharacter)


#define TetriTest_Source_TetriTest_TetriTestCharacter_h_26_INCLASS \
private: \
	static void StaticRegisterNativesATetriTestCharacter(); \
	friend struct Z_Construct_UClass_ATetriTestCharacter_Statics; \
public: \
	DECLARE_CLASS(ATetriTestCharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/TetriTest"), NO_API) \
	DECLARE_SERIALIZER(ATetriTestCharacter)


#define TetriTest_Source_TetriTest_TetriTestCharacter_h_26_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ATetriTestCharacter(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ATetriTestCharacter) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ATetriTestCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ATetriTestCharacter); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ATetriTestCharacter(ATetriTestCharacter&&); \
	NO_API ATetriTestCharacter(const ATetriTestCharacter&); \
public:


#define TetriTest_Source_TetriTest_TetriTestCharacter_h_26_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ATetriTestCharacter(ATetriTestCharacter&&); \
	NO_API ATetriTestCharacter(const ATetriTestCharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ATetriTestCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ATetriTestCharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ATetriTestCharacter)


#define TetriTest_Source_TetriTest_TetriTestCharacter_h_26_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__currentMode() { return STRUCT_OFFSET(ATetriTestCharacter, currentMode); } \
	FORCEINLINE static uint32 __PPO__Mesh1P() { return STRUCT_OFFSET(ATetriTestCharacter, Mesh1P); } \
	FORCEINLINE static uint32 __PPO__FP_Gun() { return STRUCT_OFFSET(ATetriTestCharacter, FP_Gun); } \
	FORCEINLINE static uint32 __PPO__FP_MuzzleLocation() { return STRUCT_OFFSET(ATetriTestCharacter, FP_MuzzleLocation); } \
	FORCEINLINE static uint32 __PPO__VR_Gun() { return STRUCT_OFFSET(ATetriTestCharacter, VR_Gun); } \
	FORCEINLINE static uint32 __PPO__VR_MuzzleLocation() { return STRUCT_OFFSET(ATetriTestCharacter, VR_MuzzleLocation); } \
	FORCEINLINE static uint32 __PPO__FirstPersonCameraComponent() { return STRUCT_OFFSET(ATetriTestCharacter, FirstPersonCameraComponent); } \
	FORCEINLINE static uint32 __PPO__R_MotionController() { return STRUCT_OFFSET(ATetriTestCharacter, R_MotionController); } \
	FORCEINLINE static uint32 __PPO__L_MotionController() { return STRUCT_OFFSET(ATetriTestCharacter, L_MotionController); }


#define TetriTest_Source_TetriTest_TetriTestCharacter_h_19_PROLOG
#define TetriTest_Source_TetriTest_TetriTestCharacter_h_26_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	TetriTest_Source_TetriTest_TetriTestCharacter_h_26_PRIVATE_PROPERTY_OFFSET \
	TetriTest_Source_TetriTest_TetriTestCharacter_h_26_SPARSE_DATA \
	TetriTest_Source_TetriTest_TetriTestCharacter_h_26_RPC_WRAPPERS \
	TetriTest_Source_TetriTest_TetriTestCharacter_h_26_INCLASS \
	TetriTest_Source_TetriTest_TetriTestCharacter_h_26_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define TetriTest_Source_TetriTest_TetriTestCharacter_h_26_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	TetriTest_Source_TetriTest_TetriTestCharacter_h_26_PRIVATE_PROPERTY_OFFSET \
	TetriTest_Source_TetriTest_TetriTestCharacter_h_26_SPARSE_DATA \
	TetriTest_Source_TetriTest_TetriTestCharacter_h_26_RPC_WRAPPERS_NO_PURE_DECLS \
	TetriTest_Source_TetriTest_TetriTestCharacter_h_26_INCLASS_NO_PURE_DECLS \
	TetriTest_Source_TetriTest_TetriTestCharacter_h_26_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> TETRITEST_API UClass* StaticClass<class ATetriTestCharacter>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID TetriTest_Source_TetriTest_TetriTestCharacter_h


#define FOREACH_ENUM_MODE(op) \
	op(mode::push) \
	op(mode::rotate) \
	op(mode::destroy) 

enum class mode;
template<> TETRITEST_API UEnum* StaticEnum<mode>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
