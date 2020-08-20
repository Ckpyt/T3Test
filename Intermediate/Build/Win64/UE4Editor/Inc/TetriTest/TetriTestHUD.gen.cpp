// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "TetriTest/TetriTestHUD.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTetriTestHUD() {}
// Cross Module References
	TETRITEST_API UClass* Z_Construct_UClass_ATetriTestHUD_NoRegister();
	TETRITEST_API UClass* Z_Construct_UClass_ATetriTestHUD();
	ENGINE_API UClass* Z_Construct_UClass_AHUD();
	UPackage* Z_Construct_UPackage__Script_TetriTest();
// End Cross Module References
	void ATetriTestHUD::StaticRegisterNativesATetriTestHUD()
	{
	}
	UClass* Z_Construct_UClass_ATetriTestHUD_NoRegister()
	{
		return ATetriTestHUD::StaticClass();
	}
	struct Z_Construct_UClass_ATetriTestHUD_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ATetriTestHUD_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AHUD,
		(UObject* (*)())Z_Construct_UPackage__Script_TetriTest,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ATetriTestHUD_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Rendering Actor Input Replication" },
		{ "IncludePath", "TetriTestHUD.h" },
		{ "ModuleRelativePath", "TetriTestHUD.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ATetriTestHUD_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATetriTestHUD>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ATetriTestHUD_Statics::ClassParams = {
		&ATetriTestHUD::StaticClass,
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
		0x008002ACu,
		METADATA_PARAMS(Z_Construct_UClass_ATetriTestHUD_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ATetriTestHUD_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ATetriTestHUD()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ATetriTestHUD_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ATetriTestHUD, 1447923612);
	template<> TETRITEST_API UClass* StaticClass<ATetriTestHUD>()
	{
		return ATetriTestHUD::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ATetriTestHUD(Z_Construct_UClass_ATetriTestHUD, &ATetriTestHUD::StaticClass, TEXT("/Script/TetriTest"), TEXT("ATetriTestHUD"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ATetriTestHUD);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
