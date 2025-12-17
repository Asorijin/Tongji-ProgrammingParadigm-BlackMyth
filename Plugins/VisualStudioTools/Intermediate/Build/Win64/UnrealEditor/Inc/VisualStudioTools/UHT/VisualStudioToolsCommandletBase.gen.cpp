// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VisualStudioTools/Private/VisualStudioToolsCommandletBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVisualStudioToolsCommandletBase() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UCommandlet();
UPackage* Z_Construct_UPackage__Script_VisualStudioTools();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVisualStudioToolsCommandletBase();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVisualStudioToolsCommandletBase_NoRegister();
// End Cross Module References

// Begin Class UVisualStudioToolsCommandletBase
void UVisualStudioToolsCommandletBase::StaticRegisterNativesUVisualStudioToolsCommandletBase()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UVisualStudioToolsCommandletBase);
UClass* Z_Construct_UClass_UVisualStudioToolsCommandletBase_NoRegister()
{
	return UVisualStudioToolsCommandletBase::StaticClass();
}
struct Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "VisualStudioToolsCommandletBase.h" },
		{ "ModuleRelativePath", "Private/VisualStudioToolsCommandletBase.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVisualStudioToolsCommandletBase>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UCommandlet,
	(UObject* (*)())Z_Construct_UPackage__Script_VisualStudioTools,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics::ClassParams = {
	&UVisualStudioToolsCommandletBase::StaticClass,
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
	0x000000A9u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics::Class_MetaDataParams), Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UVisualStudioToolsCommandletBase()
{
	if (!Z_Registration_Info_UClass_UVisualStudioToolsCommandletBase.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVisualStudioToolsCommandletBase.OuterSingleton, Z_Construct_UClass_UVisualStudioToolsCommandletBase_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVisualStudioToolsCommandletBase.OuterSingleton;
}
template<> VISUALSTUDIOTOOLS_API UClass* StaticClass<UVisualStudioToolsCommandletBase>()
{
	return UVisualStudioToolsCommandletBase::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UVisualStudioToolsCommandletBase);
UVisualStudioToolsCommandletBase::~UVisualStudioToolsCommandletBase() {}
// End Class UVisualStudioToolsCommandletBase

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_ttqz_BlackMyth_Tongji_ProgrammingParadigm_BlackMyth_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsCommandletBase_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVisualStudioToolsCommandletBase, UVisualStudioToolsCommandletBase::StaticClass, TEXT("UVisualStudioToolsCommandletBase"), &Z_Registration_Info_UClass_UVisualStudioToolsCommandletBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVisualStudioToolsCommandletBase), 570926285U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_ttqz_BlackMyth_Tongji_ProgrammingParadigm_BlackMyth_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsCommandletBase_h_1914996749(TEXT("/Script/VisualStudioTools"),
	Z_CompiledInDeferFile_FID_Users_ttqz_BlackMyth_Tongji_ProgrammingParadigm_BlackMyth_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsCommandletBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_ttqz_BlackMyth_Tongji_ProgrammingParadigm_BlackMyth_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VisualStudioToolsCommandletBase_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
