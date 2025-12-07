// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "black_money/Monster.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeMonster() {}

// Begin Cross Module References
BLACK_MONEY_API UClass* Z_Construct_UClass_AMonster();
BLACK_MONEY_API UClass* Z_Construct_UClass_AMonster_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_APawn();
UPackage* Z_Construct_UPackage__Script_black_money();
// End Cross Module References

// Begin Class AMonster
void AMonster::StaticRegisterNativesAMonster()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AMonster);
UClass* Z_Construct_UClass_AMonster_NoRegister()
{
	return AMonster::StaticClass();
}
struct Z_Construct_UClass_AMonster_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "Monster.h" },
		{ "ModuleRelativePath", "Monster.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AMonster>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AMonster_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_APawn,
	(UObject* (*)())Z_Construct_UPackage__Script_black_money,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AMonster_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AMonster_Statics::ClassParams = {
	&AMonster::StaticClass,
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
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AMonster_Statics::Class_MetaDataParams), Z_Construct_UClass_AMonster_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AMonster()
{
	if (!Z_Registration_Info_UClass_AMonster.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AMonster.OuterSingleton, Z_Construct_UClass_AMonster_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AMonster.OuterSingleton;
}
template<> BLACK_MONEY_API UClass* StaticClass<AMonster>()
{
	return AMonster::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AMonster);
AMonster::~AMonster() {}
// End Class AMonster

// Begin Registration
struct Z_CompiledInDeferFile_FID_Repo_Source_black_money_Monster_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AMonster, AMonster::StaticClass, TEXT("AMonster"), &Z_Registration_Info_UClass_AMonster, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AMonster), 2938769460U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Repo_Source_black_money_Monster_h_1955098786(TEXT("/Script/black_money"),
	Z_CompiledInDeferFile_FID_Repo_Source_black_money_Monster_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Repo_Source_black_money_Monster_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
