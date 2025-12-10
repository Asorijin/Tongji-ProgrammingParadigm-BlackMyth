// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "black_money/Public/Tools.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTools() {}

// Begin Cross Module References
BLACK_MONEY_API UClass* Z_Construct_UClass_ATools();
BLACK_MONEY_API UClass* Z_Construct_UClass_ATools_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AActor();
UPackage* Z_Construct_UPackage__Script_black_money();
// End Cross Module References

// Begin Class ATools
void ATools::StaticRegisterNativesATools()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ATools);
UClass* Z_Construct_UClass_ATools_NoRegister()
{
	return ATools::StaticClass();
}
struct Z_Construct_UClass_ATools_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "Tools.h" },
		{ "ModuleRelativePath", "Public/Tools.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_toolNumber_MetaData[] = {
		{ "ModuleRelativePath", "Public/Tools.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_toolNumber;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ATools>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_ATools_Statics::NewProp_toolNumber = { "toolNumber", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ATools, toolNumber), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_toolNumber_MetaData), NewProp_toolNumber_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ATools_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ATools_Statics::NewProp_toolNumber,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ATools_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ATools_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_black_money,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ATools_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ATools_Statics::ClassParams = {
	&ATools::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_ATools_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_ATools_Statics::PropPointers),
	0,
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ATools_Statics::Class_MetaDataParams), Z_Construct_UClass_ATools_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ATools()
{
	if (!Z_Registration_Info_UClass_ATools.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ATools.OuterSingleton, Z_Construct_UClass_ATools_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ATools.OuterSingleton;
}
template<> BLACK_MONEY_API UClass* StaticClass<ATools>()
{
	return ATools::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ATools);
ATools::~ATools() {}
// End Class ATools

// Begin Registration
struct Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_Tools_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ATools, ATools::StaticClass, TEXT("ATools"), &Z_Registration_Info_UClass_ATools, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ATools), 2518177540U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_Tools_h_2191551998(TEXT("/Script/black_money"),
	Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_Tools_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_Tools_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
