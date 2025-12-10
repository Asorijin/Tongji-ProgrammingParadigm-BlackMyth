// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "black_money/Public/ToolHp.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeToolHp() {}

// Begin Cross Module References
BLACK_MONEY_API UClass* Z_Construct_UClass_AToolHp();
BLACK_MONEY_API UClass* Z_Construct_UClass_AToolHp_NoRegister();
BLACK_MONEY_API UClass* Z_Construct_UClass_ATools();
UPackage* Z_Construct_UPackage__Script_black_money();
// End Cross Module References

// Begin Class AToolHp
void AToolHp::StaticRegisterNativesAToolHp()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AToolHp);
UClass* Z_Construct_UClass_AToolHp_NoRegister()
{
	return AToolHp::StaticClass();
}
struct Z_Construct_UClass_AToolHp_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "ToolHp.h" },
		{ "ModuleRelativePath", "Public/ToolHp.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AToolHp>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_AToolHp_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ATools,
	(UObject* (*)())Z_Construct_UPackage__Script_black_money,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_AToolHp_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_AToolHp_Statics::ClassParams = {
	&AToolHp::StaticClass,
	"Engine",
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_AToolHp_Statics::Class_MetaDataParams), Z_Construct_UClass_AToolHp_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_AToolHp()
{
	if (!Z_Registration_Info_UClass_AToolHp.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AToolHp.OuterSingleton, Z_Construct_UClass_AToolHp_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_AToolHp.OuterSingleton;
}
template<> BLACK_MONEY_API UClass* StaticClass<AToolHp>()
{
	return AToolHp::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(AToolHp);
AToolHp::~AToolHp() {}
// End Class AToolHp

// Begin Registration
struct Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_ToolHp_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_AToolHp, AToolHp::StaticClass, TEXT("AToolHp"), &Z_Registration_Info_UClass_AToolHp, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AToolHp), 2798053788U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_ToolHp_h_552783662(TEXT("/Script/black_money"),
	Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_ToolHp_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_ToolHp_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
