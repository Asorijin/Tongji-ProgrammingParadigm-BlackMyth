// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "black_money/Public/TestInstance.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTestInstance() {}

// Begin Cross Module References
BLACK_MONEY_API UClass* Z_Construct_UClass_UTestInstance();
BLACK_MONEY_API UClass* Z_Construct_UClass_UTestInstance_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UGameInstance();
UPackage* Z_Construct_UPackage__Script_black_money();
// End Cross Module References

// Begin Class UTestInstance
void UTestInstance::StaticRegisterNativesUTestInstance()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UTestInstance);
UClass* Z_Construct_UClass_UTestInstance_NoRegister()
{
	return UTestInstance::StaticClass();
}
struct Z_Construct_UClass_UTestInstance_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "TestInstance.h" },
		{ "ModuleRelativePath", "Public/TestInstance.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UTestInstance>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UTestInstance_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGameInstance,
	(UObject* (*)())Z_Construct_UPackage__Script_black_money,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UTestInstance_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UTestInstance_Statics::ClassParams = {
	&UTestInstance::StaticClass,
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
	0x009000A8u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UTestInstance_Statics::Class_MetaDataParams), Z_Construct_UClass_UTestInstance_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UTestInstance()
{
	if (!Z_Registration_Info_UClass_UTestInstance.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UTestInstance.OuterSingleton, Z_Construct_UClass_UTestInstance_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UTestInstance.OuterSingleton;
}
template<> BLACK_MONEY_API UClass* StaticClass<UTestInstance>()
{
	return UTestInstance::StaticClass();
}
UTestInstance::UTestInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UTestInstance);
UTestInstance::~UTestInstance() {}
// End Class UTestInstance

// Begin Registration
struct Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_TestInstance_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UTestInstance, UTestInstance::StaticClass, TEXT("UTestInstance"), &Z_Registration_Info_UClass_UTestInstance, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UTestInstance), 452828431U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_TestInstance_h_3388087568(TEXT("/Script/black_money"),
	Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_TestInstance_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_TestInstance_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
