// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "black_money/Public/LandTemple.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeLandTemple() {}

// Begin Cross Module References
BLACK_MONEY_API UClass* Z_Construct_UClass_ALandTemple();
BLACK_MONEY_API UClass* Z_Construct_UClass_ALandTemple_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_AActor();
ENGINE_API UClass* Z_Construct_UClass_UStaticMeshComponent_NoRegister();
UPackage* Z_Construct_UPackage__Script_black_money();
// End Cross Module References

// Begin Class ALandTemple
void ALandTemple::StaticRegisterNativesALandTemple()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ALandTemple);
UClass* Z_Construct_UClass_ALandTemple_NoRegister()
{
	return ALandTemple::StaticClass();
}
struct Z_Construct_UClass_ALandTemple_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "LandTemple.h" },
		{ "ModuleRelativePath", "Public/LandTemple.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_meshComponent_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Components" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/LandTemple.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_meshComponent;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ALandTemple>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_ALandTemple_Statics::NewProp_meshComponent = { "meshComponent", nullptr, (EPropertyFlags)0x00400000000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(ALandTemple, meshComponent), Z_Construct_UClass_UStaticMeshComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_meshComponent_MetaData), NewProp_meshComponent_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_ALandTemple_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_ALandTemple_Statics::NewProp_meshComponent,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ALandTemple_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_ALandTemple_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_AActor,
	(UObject* (*)())Z_Construct_UPackage__Script_black_money,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_ALandTemple_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_ALandTemple_Statics::ClassParams = {
	&ALandTemple::StaticClass,
	"Engine",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_ALandTemple_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_ALandTemple_Statics::PropPointers),
	0,
	0x009000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_ALandTemple_Statics::Class_MetaDataParams), Z_Construct_UClass_ALandTemple_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_ALandTemple()
{
	if (!Z_Registration_Info_UClass_ALandTemple.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ALandTemple.OuterSingleton, Z_Construct_UClass_ALandTemple_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_ALandTemple.OuterSingleton;
}
template<> BLACK_MONEY_API UClass* StaticClass<ALandTemple>()
{
	return ALandTemple::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(ALandTemple);
ALandTemple::~ALandTemple() {}
// End Class ALandTemple

// Begin Registration
struct Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_LandTemple_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_ALandTemple, ALandTemple::StaticClass, TEXT("ALandTemple"), &Z_Registration_Info_UClass_ALandTemple, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ALandTemple), 757433563U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_LandTemple_h_4078552451(TEXT("/Script/black_money"),
	Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_LandTemple_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_LandTemple_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
