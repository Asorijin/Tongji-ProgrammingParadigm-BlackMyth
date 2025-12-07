// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "black_money/black_moneyGameMode.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeblack_moneyGameMode() {}

// Begin Cross Module References
BLACK_MONEY_API UClass* Z_Construct_UClass_Ablack_moneyGameMode();
BLACK_MONEY_API UClass* Z_Construct_UClass_Ablack_moneyGameMode_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
UPackage* Z_Construct_UPackage__Script_black_money();
// End Cross Module References

// Begin Class Ablack_moneyGameMode
void Ablack_moneyGameMode::StaticRegisterNativesAblack_moneyGameMode()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(Ablack_moneyGameMode);
UClass* Z_Construct_UClass_Ablack_moneyGameMode_NoRegister()
{
	return Ablack_moneyGameMode::StaticClass();
}
struct Z_Construct_UClass_Ablack_moneyGameMode_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering HLOD WorldPartition DataLayers Transformation" },
		{ "IncludePath", "black_moneyGameMode.h" },
		{ "ModuleRelativePath", "black_moneyGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<Ablack_moneyGameMode>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_Ablack_moneyGameMode_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AGameModeBase,
	(UObject* (*)())Z_Construct_UPackage__Script_black_money,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_Ablack_moneyGameMode_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_Ablack_moneyGameMode_Statics::ClassParams = {
	&Ablack_moneyGameMode::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_Ablack_moneyGameMode_Statics::Class_MetaDataParams), Z_Construct_UClass_Ablack_moneyGameMode_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_Ablack_moneyGameMode()
{
	if (!Z_Registration_Info_UClass_Ablack_moneyGameMode.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_Ablack_moneyGameMode.OuterSingleton, Z_Construct_UClass_Ablack_moneyGameMode_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_Ablack_moneyGameMode.OuterSingleton;
}
template<> BLACK_MONEY_API UClass* StaticClass<Ablack_moneyGameMode>()
{
	return Ablack_moneyGameMode::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(Ablack_moneyGameMode);
Ablack_moneyGameMode::~Ablack_moneyGameMode() {}
// End Class Ablack_moneyGameMode

// Begin Registration
struct Z_CompiledInDeferFile_FID_Repo_Source_black_money_black_moneyGameMode_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_Ablack_moneyGameMode, Ablack_moneyGameMode::StaticClass, TEXT("Ablack_moneyGameMode"), &Z_Registration_Info_UClass_Ablack_moneyGameMode, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(Ablack_moneyGameMode), 1279691557U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Repo_Source_black_money_black_moneyGameMode_h_717812827(TEXT("/Script/black_money"),
	Z_CompiledInDeferFile_FID_Repo_Source_black_money_black_moneyGameMode_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Repo_Source_black_money_black_moneyGameMode_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
