// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VisualStudioTools/Private/BlueprintReferencesCommandlet.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBlueprintReferencesCommandlet() {}

// Begin Cross Module References
UPackage* Z_Construct_UPackage__Script_VisualStudioTools();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVisualStudioToolsCommandletBase();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVsBlueprintReferencesCommandlet();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVsBlueprintReferencesCommandlet_NoRegister();
// End Cross Module References

// Begin Class UVsBlueprintReferencesCommandlet
void UVsBlueprintReferencesCommandlet::StaticRegisterNativesUVsBlueprintReferencesCommandlet()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UVsBlueprintReferencesCommandlet);
UClass* Z_Construct_UClass_UVsBlueprintReferencesCommandlet_NoRegister()
{
	return UVsBlueprintReferencesCommandlet::StaticClass();
}
struct Z_Construct_UClass_UVsBlueprintReferencesCommandlet_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "BlueprintReferencesCommandlet.h" },
		{ "ModuleRelativePath", "Private/BlueprintReferencesCommandlet.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVsBlueprintReferencesCommandlet>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UVsBlueprintReferencesCommandlet_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UVisualStudioToolsCommandletBase,
	(UObject* (*)())Z_Construct_UPackage__Script_VisualStudioTools,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVsBlueprintReferencesCommandlet_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVsBlueprintReferencesCommandlet_Statics::ClassParams = {
	&UVsBlueprintReferencesCommandlet::StaticClass,
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
	0x000000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVsBlueprintReferencesCommandlet_Statics::Class_MetaDataParams), Z_Construct_UClass_UVsBlueprintReferencesCommandlet_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UVsBlueprintReferencesCommandlet()
{
	if (!Z_Registration_Info_UClass_UVsBlueprintReferencesCommandlet.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVsBlueprintReferencesCommandlet.OuterSingleton, Z_Construct_UClass_UVsBlueprintReferencesCommandlet_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVsBlueprintReferencesCommandlet.OuterSingleton;
}
template<> VISUALSTUDIOTOOLS_API UClass* StaticClass<UVsBlueprintReferencesCommandlet>()
{
	return UVsBlueprintReferencesCommandlet::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UVsBlueprintReferencesCommandlet);
UVsBlueprintReferencesCommandlet::~UVsBlueprintReferencesCommandlet() {}
// End Class UVsBlueprintReferencesCommandlet

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_ttqz_BlackMyth_Tongji_ProgrammingParadigm_BlackMyth_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_BlueprintReferencesCommandlet_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVsBlueprintReferencesCommandlet, UVsBlueprintReferencesCommandlet::StaticClass, TEXT("UVsBlueprintReferencesCommandlet"), &Z_Registration_Info_UClass_UVsBlueprintReferencesCommandlet, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVsBlueprintReferencesCommandlet), 1569199313U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_ttqz_BlackMyth_Tongji_ProgrammingParadigm_BlackMyth_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_BlueprintReferencesCommandlet_h_3881421657(TEXT("/Script/VisualStudioTools"),
	Z_CompiledInDeferFile_FID_Users_ttqz_BlackMyth_Tongji_ProgrammingParadigm_BlackMyth_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_BlueprintReferencesCommandlet_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_ttqz_BlackMyth_Tongji_ProgrammingParadigm_BlackMyth_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_BlueprintReferencesCommandlet_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
