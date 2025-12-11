// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "black_money/Public/EventCenter.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEventCenter() {}

// Begin Cross Module References
BLACK_MONEY_API UClass* Z_Construct_UClass_UEventCenter();
BLACK_MONEY_API UClass* Z_Construct_UClass_UEventCenter_NoRegister();
BLACK_MONEY_API UScriptStruct* Z_Construct_UScriptStruct_FToolsNumber();
COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
UPackage* Z_Construct_UPackage__Script_black_money();
// End Cross Module References

// Begin ScriptStruct FToolsNumber
static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_ToolsNumber;
class UScriptStruct* FToolsNumber::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_ToolsNumber.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_ToolsNumber.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FToolsNumber, (UObject*)Z_Construct_UPackage__Script_black_money(), TEXT("ToolsNumber"));
	}
	return Z_Registration_Info_UScriptStruct_ToolsNumber.OuterSingleton;
}
template<> BLACK_MONEY_API UScriptStruct* StaticStruct<FToolsNumber>()
{
	return FToolsNumber::StaticStruct();
}
struct Z_Construct_UScriptStruct_FToolsNumber_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/EventCenter.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_hpTools_MetaData[] = {
		{ "ModuleRelativePath", "Public/EventCenter.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FIntPropertyParams NewProp_hpTools;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static void* NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FToolsNumber>();
	}
	static const UECodeGen_Private::FStructParams StructParams;
};
const UECodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FToolsNumber_Statics::NewProp_hpTools = { "hpTools", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(FToolsNumber, hpTools), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_hpTools_MetaData), NewProp_hpTools_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FToolsNumber_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FToolsNumber_Statics::NewProp_hpTools,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FToolsNumber_Statics::PropPointers) < 2048);
const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FToolsNumber_Statics::StructParams = {
	(UObject* (*)())Z_Construct_UPackage__Script_black_money,
	nullptr,
	&NewStructOps,
	"ToolsNumber",
	Z_Construct_UScriptStruct_FToolsNumber_Statics::PropPointers,
	UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FToolsNumber_Statics::PropPointers),
	sizeof(FToolsNumber),
	alignof(FToolsNumber),
	RF_Public|RF_Transient|RF_MarkAsNative,
	EStructFlags(0x00000001),
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FToolsNumber_Statics::Struct_MetaDataParams), Z_Construct_UScriptStruct_FToolsNumber_Statics::Struct_MetaDataParams)
};
UScriptStruct* Z_Construct_UScriptStruct_FToolsNumber()
{
	if (!Z_Registration_Info_UScriptStruct_ToolsNumber.InnerSingleton)
	{
		UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_ToolsNumber.InnerSingleton, Z_Construct_UScriptStruct_FToolsNumber_Statics::StructParams);
	}
	return Z_Registration_Info_UScriptStruct_ToolsNumber.InnerSingleton;
}
// End ScriptStruct FToolsNumber

// Begin Class UEventCenter
void UEventCenter::StaticRegisterNativesUEventCenter()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UEventCenter);
UClass* Z_Construct_UClass_UEventCenter_NoRegister()
{
	return UEventCenter::StaticClass();
}
struct Z_Construct_UClass_UEventCenter_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * \n */" },
#endif
		{ "IncludePath", "EventCenter.h" },
		{ "ModuleRelativePath", "Public/EventCenter.h" },
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UEventCenter>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UEventCenter_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UObject,
	(UObject* (*)())Z_Construct_UPackage__Script_black_money,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UEventCenter_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UEventCenter_Statics::ClassParams = {
	&UEventCenter::StaticClass,
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
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UEventCenter_Statics::Class_MetaDataParams), Z_Construct_UClass_UEventCenter_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UEventCenter()
{
	if (!Z_Registration_Info_UClass_UEventCenter.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UEventCenter.OuterSingleton, Z_Construct_UClass_UEventCenter_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UEventCenter.OuterSingleton;
}
template<> BLACK_MONEY_API UClass* StaticClass<UEventCenter>()
{
	return UEventCenter::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(UEventCenter);
UEventCenter::~UEventCenter() {}
// End Class UEventCenter

// Begin Registration
struct Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_EventCenter_h_Statics
{
	static constexpr FStructRegisterCompiledInInfo ScriptStructInfo[] = {
		{ FToolsNumber::StaticStruct, Z_Construct_UScriptStruct_FToolsNumber_Statics::NewStructOps, TEXT("ToolsNumber"), &Z_Registration_Info_UScriptStruct_ToolsNumber, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FToolsNumber), 187573102U) },
	};
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UEventCenter, UEventCenter::StaticClass, TEXT("UEventCenter"), &Z_Registration_Info_UClass_UEventCenter, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UEventCenter), 15532321U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_EventCenter_h_3083593225(TEXT("/Script/black_money"),
	Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_EventCenter_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_EventCenter_h_Statics::ClassInfo),
	Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_EventCenter_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_EventCenter_h_Statics::ScriptStructInfo),
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
