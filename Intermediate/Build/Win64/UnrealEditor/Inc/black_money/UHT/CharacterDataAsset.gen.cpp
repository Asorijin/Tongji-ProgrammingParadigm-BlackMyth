// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "black_money/Public/CharacterDataAsset.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCharacterDataAsset() {}

// Begin Cross Module References
BLACK_MONEY_API UClass* Z_Construct_UClass_UCharacterDataAsset();
BLACK_MONEY_API UClass* Z_Construct_UClass_UCharacterDataAsset_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UDataAsset();
UPackage* Z_Construct_UPackage__Script_black_money();
// End Cross Module References

// Begin Class UCharacterDataAsset
void UCharacterDataAsset::StaticRegisterNativesUCharacterDataAsset()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCharacterDataAsset);
UClass* Z_Construct_UClass_UCharacterDataAsset_NoRegister()
{
	return UCharacterDataAsset::StaticClass();
}
struct Z_Construct_UClass_UCharacterDataAsset_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "CharacterDataAsset.h" },
		{ "ModuleRelativePath", "Public/CharacterDataAsset.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCharacterDataAsset>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UCharacterDataAsset_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UDataAsset,
	(UObject* (*)())Z_Construct_UPackage__Script_black_money,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCharacterDataAsset_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UCharacterDataAsset_Statics::ClassParams = {
	&UCharacterDataAsset::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	0,
	0,
	0x001000A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UCharacterDataAsset_Statics::Class_MetaDataParams), Z_Construct_UClass_UCharacterDataAsset_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UCharacterDataAsset()
{
	if (!Z_Registration_Info_UClass_UCharacterDataAsset.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCharacterDataAsset.OuterSingleton, Z_Construct_UClass_UCharacterDataAsset_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UCharacterDataAsset.OuterSingleton;
}
template<> BLACK_MONEY_API UClass* StaticClass<UCharacterDataAsset>()
{
	return UCharacterDataAsset::StaticClass();
}
UCharacterDataAsset::UCharacterDataAsset(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UCharacterDataAsset);
UCharacterDataAsset::~UCharacterDataAsset() {}
// End Class UCharacterDataAsset

// Begin Registration
struct Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_CharacterDataAsset_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UCharacterDataAsset, UCharacterDataAsset::StaticClass, TEXT("UCharacterDataAsset"), &Z_Registration_Info_UClass_UCharacterDataAsset, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCharacterDataAsset), 833355759U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_CharacterDataAsset_h_2770351488(TEXT("/Script/black_money"),
	Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_CharacterDataAsset_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_CharacterDataAsset_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
