// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "VisualStudioTools/Private/VSServerCommandlet.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVSServerCommandlet() {}

// Begin Cross Module References
ENGINE_API UClass* Z_Construct_UClass_UCommandlet();
UPackage* Z_Construct_UPackage__Script_VisualStudioTools();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVSServerCommandlet();
VISUALSTUDIOTOOLS_API UClass* Z_Construct_UClass_UVSServerCommandlet_NoRegister();
// End Cross Module References

// Begin Class UVSServerCommandlet
void UVSServerCommandlet::StaticRegisterNativesUVSServerCommandlet()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UVSServerCommandlet);
UClass* Z_Construct_UClass_UVSServerCommandlet_NoRegister()
{
	return UVSServerCommandlet::StaticClass();
}
struct Z_Construct_UClass_UVSServerCommandlet_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "IncludePath", "VSServerCommandlet.h" },
		{ "ModuleRelativePath", "Private/VSServerCommandlet.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVSServerCommandlet>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UVSServerCommandlet_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UCommandlet,
	(UObject* (*)())Z_Construct_UPackage__Script_VisualStudioTools,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UVSServerCommandlet_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UVSServerCommandlet_Statics::ClassParams = {
	&UVSServerCommandlet::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UVSServerCommandlet_Statics::Class_MetaDataParams), Z_Construct_UClass_UVSServerCommandlet_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UVSServerCommandlet()
{
	if (!Z_Registration_Info_UClass_UVSServerCommandlet.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVSServerCommandlet.OuterSingleton, Z_Construct_UClass_UVSServerCommandlet_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UVSServerCommandlet.OuterSingleton;
}
template<> VISUALSTUDIOTOOLS_API UClass* StaticClass<UVSServerCommandlet>()
{
	return UVSServerCommandlet::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UVSServerCommandlet);
UVSServerCommandlet::~UVSServerCommandlet() {}
// End Class UVSServerCommandlet

// Begin Registration
struct Z_CompiledInDeferFile_FID_Users_ttqz_BlackMyth_Tongji_ProgrammingParadigm_BlackMyth_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VSServerCommandlet_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UVSServerCommandlet, UVSServerCommandlet::StaticClass, TEXT("UVSServerCommandlet"), &Z_Registration_Info_UClass_UVSServerCommandlet, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVSServerCommandlet), 2634532557U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Users_ttqz_BlackMyth_Tongji_ProgrammingParadigm_BlackMyth_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VSServerCommandlet_h_854072527(TEXT("/Script/VisualStudioTools"),
	Z_CompiledInDeferFile_FID_Users_ttqz_BlackMyth_Tongji_ProgrammingParadigm_BlackMyth_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VSServerCommandlet_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Users_ttqz_BlackMyth_Tongji_ProgrammingParadigm_BlackMyth_Plugins_VisualStudioTools_Source_VisualStudioTools_Private_VSServerCommandlet_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
