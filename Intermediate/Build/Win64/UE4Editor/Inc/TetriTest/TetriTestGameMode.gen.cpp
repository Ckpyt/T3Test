// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TetriTest/TetriTestGameMode.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTetriTestGameMode() {}
// Cross Module References
	TETRITEST_API UClass* Z_Construct_UClass_ATetriTestGameMode_NoRegister();
	TETRITEST_API UClass* Z_Construct_UClass_ATetriTestGameMode();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_TetriTest();
// End Cross Module References
	void ATetriTestGameMode::StaticRegisterNativesATetriTestGameMode()
	{
	}
	UClass* Z_Construct_UClass_ATetriTestGameMode_NoRegister()
	{
		return ATetriTestGameMode::StaticClass();
	}
	struct Z_Construct_UClass_ATetriTestGameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ATetriTestGameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_TetriTest,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATetriTestGameMode_Statics::Class_MetaDataParams[] = {
		{ "Comment", "//#define SCENE_SIZE 4\n//#define SCENE_HEIGHT 20\n//#define _MAX_HEIGHT_ 19000.f\n" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "TetriTestGameMode.h" },
		{ "ModuleRelativePath", "TetriTestGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
		{ "ToolTip", "#define SCENE_SIZE 4\n#define SCENE_HEIGHT 20\n#define _MAX_HEIGHT_ 19000.f" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ATetriTestGameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATetriTestGameMode>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ATetriTestGameMode_Statics::ClassParams = {
		&ATetriTestGameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802ACu,
		METADATA_PARAMS(Z_Construct_UClass_ATetriTestGameMode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ATetriTestGameMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ATetriTestGameMode()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ATetriTestGameMode_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ATetriTestGameMode, 878876119);
	template<> TETRITEST_API UClass* StaticClass<ATetriTestGameMode>()
	{
		return ATetriTestGameMode::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ATetriTestGameMode(Z_Construct_UClass_ATetriTestGameMode, &ATetriTestGameMode::StaticClass, TEXT("/Script/TetriTest"), TEXT("ATetriTestGameMode"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ATetriTestGameMode);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
