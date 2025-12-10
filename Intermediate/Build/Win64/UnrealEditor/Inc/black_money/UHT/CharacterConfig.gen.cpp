// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "black_money/Public/CharacterConfig.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCharacterConfig() {}

// Begin Cross Module References
BLACK_MONEY_API UClass* Z_Construct_UClass_UCharacterConfig();
BLACK_MONEY_API UClass* Z_Construct_UClass_UCharacterConfig_NoRegister();
BLACK_MONEY_API UScriptStruct* Z_Construct_UScriptStruct_FEquipment();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
UPackage* Z_Construct_UPackage__Script_black_money();
// End Cross Module References

// Begin ScriptStruct FEquipment
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_Equipment;
class UScriptStruct* FEquipment::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_Equipment.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_Equipment.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FEquipment, (UObject*)Z_Construct_UPackage__Script_black_money(), TEXT("Equipment"));
	}
	return Z_Registration_Info_UScriptStruct_Equipment.OuterSingleton;
}
template<> BLACK_MONEY_API UScriptStruct* StaticStruct<FEquipment>()
{
	return FEquipment::StaticStruct();
}
struct Z_Construct_UScriptStruct_FEquipment_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/CharacterConfig.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_head_MetaData[] = {
		{ "ModuleRelativePath", "Public/CharacterConfig.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_body_MetaData[] = {
		{ "ModuleRelativePath", "Public/CharacterConfig.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_boot_MetaData[] = {
		{ "ModuleRelativePath", "Public/CharacterConfig.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_weapon_MetaData[] = {
		{ "ModuleRelativePath", "Public/CharacterConfig.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_head;
	static const UECodeGen_Private::FIntPropertyParams NewProp_body;
	static const UECodeGen_Private::FIntPropertyParams NewProp_boot;
	static const UECodeGen_Private::FIntPropertyParams NewProp_weapon;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FEquipment>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FEquipment_Statics::NewProp_head = { "head", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FEquipment, head), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_head_MetaData), NewProp_head_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FEquipment_Statics::NewProp_body = { "body", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FEquipment, body), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_body_MetaData), NewProp_body_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FEquipment_Statics::NewProp_boot = { "boot", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FEquipment, boot), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_boot_MetaData), NewProp_boot_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FEquipment_Statics::NewProp_weapon = { "weapon", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FEquipment, weapon), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_weapon_MetaData), NewProp_weapon_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FEquipment_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEquipment_Statics::NewProp_head,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEquipment_Statics::NewProp_body,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEquipment_Statics::NewProp_boot,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FEquipment_Statics::NewProp_weapon,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEquipment_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FEquipment_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_black_money,
	nullptr,
	&NewStructOps,
	"Equipment",
	Z_Construct_UScriptStruct_FEquipment_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEquipment_Statics::PropPointers),
	sizeof(FEquipment),
	alignof(FEquipment),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FEquipment_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FEquipment_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FEquipment()
{
	if (!Z_Registration_Info_UScriptStruct_Equipment.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_Equipment.InnerSingleton, Z_Construct_UScriptStruct_FEquipment_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_Equipment.InnerSingleton;
}
// End ScriptStruct FEquipment

// Begin Class UCharacterConfig
void UCharacterConfig::StaticRegisterNativesUCharacterConfig()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCharacterConfig);
UClass* Z_Construct_UClass_UCharacterConfig_NoRegister()
{
	return UCharacterConfig::StaticClass();
}
struct Z_Construct_UClass_UCharacterConfig_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc6\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd""Character\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xda\xb3\xef\xbf\xbd\xca\xbc\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xca\xb9\xef\xbf\xbd\xc3\xb8\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc8\xa1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc4\xbc\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xbc\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd6\xb5\n * \xef\xbf\xbd\xef\xbf\xbd\xcf\xb7\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xbd\xef\xbf\xbd\xc9\xab\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd6\xb5\xef\xbf\xbd\xe6\xb4\xa2\xef\xbf\xbd\xda\xb8\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\n */" },
#endif
		{ "IncludePath", "CharacterConfig.h" },
		{ "ModuleRelativePath", "Public/CharacterConfig.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc6\xbb\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd""Character\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xda\xb3\xef\xbf\xbd\xca\xbc\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xca\xb9\xef\xbf\xbd\xc3\xb8\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc8\xa1\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc4\xbc\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xbc\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd6\xb5\n\xef\xbf\xbd\xef\xbf\xbd\xcf\xb7\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd0\xbd\xef\xbf\xbd\xc9\xab\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xd6\xb5\xef\xbf\xbd\xe6\xb4\xa2\xef\xbf\xbd\xda\xb8\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCharacterConfig>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UCharacterConfig_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_black_money,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCharacterConfig_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UCharacterConfig_Statics::ClassParams = {
	&UCharacterConfig::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UCharacterConfig_Statics::Class_MetaDataParams), Z_Construct_UClass_UCharacterConfig_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UCharacterConfig()
{
	if (!Z_Registration_Info_UClass_UCharacterConfig.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCharacterConfig.OuterSingleton, Z_Construct_UClass_UCharacterConfig_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UCharacterConfig.OuterSingleton;
}
template<> BLACK_MONEY_API UClass* StaticClass<UCharacterConfig>()
{
	return UCharacterConfig::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UCharacterConfig);
UCharacterConfig::~UCharacterConfig() {}
// End Class UCharacterConfig

// Begin Registration
struct Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_CharacterConfig_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FEquipment::StaticStruct, Z_Construct_UScriptStruct_FEquipment_Statics::NewStructOps, TEXT("Equipment"), &Z_Registration_Info_UScriptStruct_Equipment, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FEquipment), 3641798650U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UCharacterConfig, UCharacterConfig::StaticClass, TEXT("UCharacterConfig"), &Z_Registration_Info_UClass_UCharacterConfig, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCharacterConfig), 3142951103U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_CharacterConfig_h_1569572883(TEXT("/Script/black_money"),
	Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_CharacterConfig_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_CharacterConfig_h_Statics::ClassInfo),
	Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_CharacterConfig_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_CharacterConfig_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
