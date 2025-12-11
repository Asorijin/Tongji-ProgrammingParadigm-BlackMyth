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
BLACK_MONEY_API UClass* Z_Construct_UClass_UCharacterConfig_NoRegister();
BLACK_MONEY_API UClass* Z_Construct_UClass_UEventCenter_NoRegister();
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
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_eventCenter_MetaData[] = {
		{ "ModuleRelativePath", "Public/black_moneyGameInstance.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_characterConfig_MetaData[] = {
		{ "ModuleRelativePath", "Public/black_moneyGameInstance.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_eventCenter;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_characterConfig;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<Ublack_moneyGameInstance>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_Ublack_moneyGameInstance_Statics::NewProp_eventCenter = { "eventCenter", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Ublack_moneyGameInstance, eventCenter), Z_Construct_UClass_UEventCenter_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_eventCenter_MetaData), NewProp_eventCenter_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_Ublack_moneyGameInstance_Statics::NewProp_characterConfig = { "characterConfig", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Ublack_moneyGameInstance, characterConfig), Z_Construct_UClass_UCharacterConfig_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_characterConfig_MetaData), NewProp_characterConfig_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_Ublack_moneyGameInstance_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_Ublack_moneyGameInstance_Statics::NewProp_eventCenter,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_Ublack_moneyGameInstance_Statics::NewProp_characterConfig,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_Ublack_moneyGameInstance_Statics::PropPointers) < 2048);
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
	Z_Construct_UClass_Ublack_moneyGameInstance_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_Ublack_moneyGameInstance_Statics::PropPointers),
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
		{ Z_Construct_UClass_Ublack_moneyGameInstance, Ublack_moneyGameInstance::StaticClass, TEXT("Ublack_moneyGameInstance"), &Z_Registration_Info_UClass_Ublack_moneyGameInstance, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(Ublack_moneyGameInstance), 3895908210U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_black_moneyGameInstance_h_1248382549(TEXT("/Script/black_money"),
	Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_black_moneyGameInstance_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_black_moneyGameInstance_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
