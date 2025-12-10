// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "black_money/Public/black_moneyGameInstance.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeblack_moneyGameInstance() {}

// Begin Cross Module References
BLACK_MONEY_API UClass* Z_Construct_UClass_Ublack_moneyGameInstance();
BLACK_MONEY_API UClass* Z_Construct_UClass_Ublack_moneyGameInstance_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_UGameInstance();
UPackage* Z_Construct_UPackage__Script_black_money();
// End Cross Module References

// Begin Class Ublack_moneyGameInstance
void Ublack_moneyGameInstance::StaticRegisterNativesUblack_moneyGameInstance()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(Ublack_moneyGameInstance);
UClass* Z_Construct_UClass_Ublack_moneyGameInstance_NoRegister()
{
	return Ublack_moneyGameInstance::StaticClass();
}
struct Z_Construct_UClass_Ublack_moneyGameInstance_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "black_moneyGameInstance.h" },
		{ "ModuleRelativePath", "Public/black_moneyGameInstance.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<Ublack_moneyGameInstance>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_Ublack_moneyGameInstance_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UGameInstance,
	(UObject* (*)())Z_Construct_UPackage__Script_black_money,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_Ublack_moneyGameInstance_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_Ublack_moneyGameInstance_Statics::ClassParams = {
	&Ublack_moneyGameInstance::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_Ublack_moneyGameInstance_Statics::Class_MetaDataParams), Z_Construct_UClass_Ublack_moneyGameInstance_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_Ublack_moneyGameInstance()
{
	if (!Z_Registration_Info_UClass_Ublack_moneyGameInstance.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_Ublack_moneyGameInstance.OuterSingleton, Z_Construct_UClass_Ublack_moneyGameInstance_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_Ublack_moneyGameInstance.OuterSingleton;
}
template<> BLACK_MONEY_API UClass* StaticClass<Ublack_moneyGameInstance>()
{
	return Ublack_moneyGameInstance::StaticClass();
}
Ublack_moneyGameInstance::Ublack_moneyGameInstance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(Ublack_moneyGameInstance);
Ublack_moneyGameInstance::~Ublack_moneyGameInstance() {}
// End Class Ublack_moneyGameInstance

// Begin Registration
struct Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_black_moneyGameInstance_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_Ublack_moneyGameInstance, Ublack_moneyGameInstance::StaticClass, TEXT("Ublack_moneyGameInstance"), &Z_Registration_Info_UClass_Ublack_moneyGameInstance, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(Ublack_moneyGameInstance), 2192457092U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_black_moneyGameInstance_h_1223550351(TEXT("/Script/black_money"),
	Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_black_moneyGameInstance_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_black_moneyGameInstance_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
