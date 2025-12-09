// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "black_money/Public/UIBasePanel.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUIBasePanel() {}

// Begin Cross Module References
BLACK_MONEY_API UClass* Z_Construct_UClass_UUIBasePanel();
BLACK_MONEY_API UClass* Z_Construct_UClass_UUIBasePanel_NoRegister();
UMG_API UClass* Z_Construct_UClass_UUserWidget();
UPackage* Z_Construct_UPackage__Script_black_money();
// End Cross Module References

// Begin Class UUIBasePanel Function HidePanel
struct Z_Construct_UFunction_UUIBasePanel_HidePanel_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UI" },
		{ "ModuleRelativePath", "Public/UIBasePanel.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUIBasePanel_HidePanel_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUIBasePanel, nullptr, "HidePanel", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUIBasePanel_HidePanel_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUIBasePanel_HidePanel_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUIBasePanel_HidePanel()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUIBasePanel_HidePanel_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUIBasePanel::execHidePanel)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->HidePanel();
	P_NATIVE_END;
}
// End Class UUIBasePanel Function HidePanel

// Begin Class UUIBasePanel Function ShowPanel
struct Z_Construct_UFunction_UUIBasePanel_ShowPanel_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[] = {
		{ "Category", "UI" },
		{ "ModuleRelativePath", "Public/UIBasePanel.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FFunctionParams FuncParams;
};
const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UUIBasePanel_ShowPanel_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UUIBasePanel, nullptr, "ShowPanel", nullptr, nullptr, nullptr, 0, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020400, 0, 0, METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UFunction_UUIBasePanel_ShowPanel_Statics::Function_MetaDataParams), Z_Construct_UFunction_UUIBasePanel_ShowPanel_Statics::Function_MetaDataParams) };
UFunction* Z_Construct_UFunction_UUIBasePanel_ShowPanel()
{
	static UFunction* ReturnFunction = nullptr;
	if (!ReturnFunction)
	{
		UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UUIBasePanel_ShowPanel_Statics::FuncParams);
	}
	return ReturnFunction;
}
DEFINE_FUNCTION(UUIBasePanel::execShowPanel)
{
	P_FINISH;
	P_NATIVE_BEGIN;
	P_THIS->ShowPanel();
	P_NATIVE_END;
}
// End Class UUIBasePanel Function ShowPanel

// Begin Class UUIBasePanel
void UUIBasePanel::StaticRegisterNativesUUIBasePanel()
{
	UClass* Class = UUIBasePanel::StaticClass();
	static const FNameNativePtrPair Funcs[] = {
		{ "HidePanel", &UUIBasePanel::execHidePanel },
		{ "ShowPanel", &UUIBasePanel::execShowPanel },
	};
	FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUIBasePanel);
UClass* Z_Construct_UClass_UUIBasePanel_NoRegister()
{
	return UUIBasePanel::StaticClass();
}
struct Z_Construct_UClass_UUIBasePanel_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/**\n * UI\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcc\xb3\xd0\xb8\xef\xbf\xbd\xef\xbf\xbd\xe0\xa3\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb5\xef\xbf\xbd\xef\xbf\xbd\n */" },
#endif
		{ "IncludePath", "UIBasePanel.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/UIBasePanel.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "UI\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcc\xb3\xd0\xb8\xef\xbf\xbd\xef\xbf\xbd\xe0\xa3\xac\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb5\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static UObject* (*const DependentSingletons[])();
	static constexpr FClassFunctionLinkInfo FuncInfo[] = {
		{ &Z_Construct_UFunction_UUIBasePanel_HidePanel, "HidePanel" }, // 1475972334
		{ &Z_Construct_UFunction_UUIBasePanel_ShowPanel, "ShowPanel" }, // 1595471910
	};
	static_assert(UE_ARRAY_COUNT(FuncInfo) < 2048);
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUIBasePanel>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
UObject* (*const Z_Construct_UClass_UUIBasePanel_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UUserWidget,
	(UObject* (*)())Z_Construct_UPackage__Script_black_money,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UUIBasePanel_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UUIBasePanel_Statics::ClassParams = {
	&UUIBasePanel::StaticClass,
	nullptr,
	&StaticCppClassTypeInfo,
	DependentSingletons,
	FuncInfo,
	nullptr,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	UE_ARRAY_COUNT(FuncInfo),
	0,
	0,
	0x00B010A0u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UUIBasePanel_Statics::Class_MetaDataParams), Z_Construct_UClass_UUIBasePanel_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UUIBasePanel()
{
	if (!Z_Registration_Info_UClass_UUIBasePanel.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUIBasePanel.OuterSingleton, Z_Construct_UClass_UUIBasePanel_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UUIBasePanel.OuterSingleton;
}
template<> BLACK_MONEY_API UClass* StaticClass<UUIBasePanel>()
{
	return UUIBasePanel::StaticClass();
}
UUIBasePanel::UUIBasePanel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UUIBasePanel);
UUIBasePanel::~UUIBasePanel() {}
// End Class UUIBasePanel

// Begin Registration
struct Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_UIBasePanel_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UUIBasePanel, UUIBasePanel::StaticClass, TEXT("UUIBasePanel"), &Z_Registration_Info_UClass_UUIBasePanel, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUIBasePanel), 968227102U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_UIBasePanel_h_1730385780(TEXT("/Script/black_money"),
	Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_UIBasePanel_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_UIBasePanel_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
