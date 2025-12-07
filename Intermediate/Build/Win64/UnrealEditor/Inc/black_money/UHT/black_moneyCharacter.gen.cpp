// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "black_money/black_moneyCharacter.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeblack_moneyCharacter() {}

// Begin Cross Module References
BLACK_MONEY_API UClass* Z_Construct_UClass_Ablack_moneyCharacter();
BLACK_MONEY_API UClass* Z_Construct_UClass_Ablack_moneyCharacter_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_ACharacter();
ENGINE_API UClass* Z_Construct_UClass_UCameraComponent_NoRegister();
ENGINE_API UClass* Z_Construct_UClass_USpringArmComponent_NoRegister();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputAction_NoRegister();
ENHANCEDINPUT_API UClass* Z_Construct_UClass_UInputMappingContext_NoRegister();
UPackage* Z_Construct_UPackage__Script_black_money();
// End Cross Module References

// Begin Class Ablack_moneyCharacter
void Ablack_moneyCharacter::StaticRegisterNativesAblack_moneyCharacter()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(Ablack_moneyCharacter);
UClass* Z_Construct_UClass_Ablack_moneyCharacter_NoRegister()
{
	return Ablack_moneyCharacter::StaticClass();
}
struct Z_Construct_UClass_Ablack_moneyCharacter_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "HideCategories", "Navigation" },
		{ "IncludePath", "black_moneyCharacter.h" },
		{ "ModuleRelativePath", "black_moneyCharacter.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_CameraBoom_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Camera" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Camera boom positioning the camera behind the character */" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "black_moneyCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Camera boom positioning the camera behind the character" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_FollowCamera_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Camera" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Follow camera */" },
#endif
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "black_moneyCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Follow camera" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DefaultMappingContext_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** MappingContext */" },
#endif
		{ "ModuleRelativePath", "black_moneyCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "MappingContext" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_JumpAction_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Jump Input Action */" },
#endif
		{ "ModuleRelativePath", "black_moneyCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Jump Input Action" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_MoveAction_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Move Input Action */" },
#endif
		{ "ModuleRelativePath", "black_moneyCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Move Input Action" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_LookAction_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** Look Input Action */" },
#endif
		{ "ModuleRelativePath", "black_moneyCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "Look Input Action" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DodgeAction_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "Input" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "// \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd DodgeAction \xd6\xb8\xef\xbf\xbd\xef\xbf\xbd\n" },
#endif
		{ "ModuleRelativePath", "black_moneyCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd DodgeAction \xd6\xb8\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DodgeStrength_MetaData[] = {
		{ "Category", "Dodge" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc7\xbf\xef\xbf\xbd\xc8\xa3\xef\xbf\xbd\xcb\xae\xc6\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd */" },
#endif
		{ "ModuleRelativePath", "black_moneyCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xc7\xbf\xef\xbf\xbd\xc8\xa3\xef\xbf\xbd\xcb\xae\xc6\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_DodgeDuration_MetaData[] = {
		{ "Category", "Dodge" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xdc\xb3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xe4\xa3\xa8\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcb\xb3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xe4\xa3\xa9 */" },
#endif
		{ "ModuleRelativePath", "black_moneyCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xdc\xb3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xe4\xa3\xa8\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcb\xb3\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xca\xb1\xef\xbf\xbd\xe4\xa3\xa9" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bIsDodging_MetaData[] = {
		{ "Category", "Dodge" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** \xef\xbf\xbd\xc7\xb7\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd */" },
#endif
		{ "ModuleRelativePath", "black_moneyCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xc7\xb7\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_bInvulnerableDuringDodge_MetaData[] = {
		{ "Category", "Dodge" },
#if !UE_BUILD_SHIPPING
		{ "Comment", "/** \xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xda\xbc\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcb\xb1\xef\xbf\xbd\xef\xbf\xbd */" },
#endif
		{ "ModuleRelativePath", "black_moneyCharacter.h" },
#if !UE_BUILD_SHIPPING
		{ "ToolTip", "\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xda\xbc\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xef\xbf\xbd\xcb\xb1\xef\xbf\xbd\xef\xbf\xbd" },
#endif
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FObjectPropertyParams NewProp_CameraBoom;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_FollowCamera;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_DefaultMappingContext;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_JumpAction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_MoveAction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_LookAction;
	static const UECodeGen_Private::FObjectPropertyParams NewProp_DodgeAction;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_DodgeStrength;
	static const UECodeGen_Private::FFloatPropertyParams NewProp_DodgeDuration;
	static void NewProp_bIsDodging_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bIsDodging;
	static void NewProp_bInvulnerableDuringDodge_SetBit(void* Obj);
	static const UECodeGen_Private::FBoolPropertyParams NewProp_bInvulnerableDuringDodge;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<Ablack_moneyCharacter>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_CameraBoom = { "CameraBoom", nullptr, (EPropertyFlags)0x00400000000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Ablack_moneyCharacter, CameraBoom), Z_Construct_UClass_USpringArmComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_CameraBoom_MetaData), NewProp_CameraBoom_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_FollowCamera = { "FollowCamera", nullptr, (EPropertyFlags)0x00400000000a001d, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Ablack_moneyCharacter, FollowCamera), Z_Construct_UClass_UCameraComponent_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_FollowCamera_MetaData), NewProp_FollowCamera_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_DefaultMappingContext = { "DefaultMappingContext", nullptr, (EPropertyFlags)0x0040000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Ablack_moneyCharacter, DefaultMappingContext), Z_Construct_UClass_UInputMappingContext_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DefaultMappingContext_MetaData), NewProp_DefaultMappingContext_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_JumpAction = { "JumpAction", nullptr, (EPropertyFlags)0x0040000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Ablack_moneyCharacter, JumpAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_JumpAction_MetaData), NewProp_JumpAction_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_MoveAction = { "MoveAction", nullptr, (EPropertyFlags)0x0040000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Ablack_moneyCharacter, MoveAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_MoveAction_MetaData), NewProp_MoveAction_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_LookAction = { "LookAction", nullptr, (EPropertyFlags)0x0040000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Ablack_moneyCharacter, LookAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_LookAction_MetaData), NewProp_LookAction_MetaData) };
const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_DodgeAction = { "DodgeAction", nullptr, (EPropertyFlags)0x0040000000000015, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Ablack_moneyCharacter, DodgeAction), Z_Construct_UClass_UInputAction_NoRegister, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DodgeAction_MetaData), NewProp_DodgeAction_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_DodgeStrength = { "DodgeStrength", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Ablack_moneyCharacter, DodgeStrength), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DodgeStrength_MetaData), NewProp_DodgeStrength_MetaData) };
const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_DodgeDuration = { "DodgeDuration", nullptr, (EPropertyFlags)0x0040000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(Ablack_moneyCharacter, DodgeDuration), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_DodgeDuration_MetaData), NewProp_DodgeDuration_MetaData) };
void Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_bIsDodging_SetBit(void* Obj)
{
	((Ablack_moneyCharacter*)Obj)->bIsDodging = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_bIsDodging = { "bIsDodging", nullptr, (EPropertyFlags)0x0040000000020001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Ablack_moneyCharacter), &Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_bIsDodging_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bIsDodging_MetaData), NewProp_bIsDodging_MetaData) };
void Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_bInvulnerableDuringDodge_SetBit(void* Obj)
{
	((Ablack_moneyCharacter*)Obj)->bInvulnerableDuringDodge = 1;
}
const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_bInvulnerableDuringDodge = { "bInvulnerableDuringDodge", nullptr, (EPropertyFlags)0x0040000000020001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, sizeof(bool), sizeof(Ablack_moneyCharacter), &Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_bInvulnerableDuringDodge_SetBit, METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_bInvulnerableDuringDodge_MetaData), NewProp_bInvulnerableDuringDodge_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_Ablack_moneyCharacter_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_CameraBoom,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_FollowCamera,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_DefaultMappingContext,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_JumpAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_MoveAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_LookAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_DodgeAction,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_DodgeStrength,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_DodgeDuration,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_bIsDodging,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_Ablack_moneyCharacter_Statics::NewProp_bInvulnerableDuringDodge,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_Ablack_moneyCharacter_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_Ablack_moneyCharacter_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_ACharacter,
	(UObject* (*)())Z_Construct_UPackage__Script_black_money,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_Ablack_moneyCharacter_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_Ablack_moneyCharacter_Statics::ClassParams = {
	&Ablack_moneyCharacter::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_Ablack_moneyCharacter_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_Ablack_moneyCharacter_Statics::PropPointers),
	0,
	0x008000A4u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_Ablack_moneyCharacter_Statics::Class_MetaDataParams), Z_Construct_UClass_Ablack_moneyCharacter_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_Ablack_moneyCharacter()
{
	if (!Z_Registration_Info_UClass_Ablack_moneyCharacter.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_Ablack_moneyCharacter.OuterSingleton, Z_Construct_UClass_Ablack_moneyCharacter_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_Ablack_moneyCharacter.OuterSingleton;
}
template<> BLACK_MONEY_API UClass* StaticClass<Ablack_moneyCharacter>()
{
	return Ablack_moneyCharacter::StaticClass();
}
DEFINE_VTABLE_PTR_HELPER_CTOR(Ablack_moneyCharacter);
Ablack_moneyCharacter::~Ablack_moneyCharacter() {}
// End Class Ablack_moneyCharacter

// Begin Registration
struct Z_CompiledInDeferFile_FID_Repo_Source_black_money_black_moneyCharacter_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_Ablack_moneyCharacter, Ablack_moneyCharacter::StaticClass, TEXT("Ablack_moneyCharacter"), &Z_Registration_Info_UClass_Ablack_moneyCharacter, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(Ablack_moneyCharacter), 3148256882U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Repo_Source_black_money_black_moneyCharacter_h_599289042(TEXT("/Script/black_money"),
	Z_CompiledInDeferFile_FID_Repo_Source_black_money_black_moneyCharacter_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Repo_Source_black_money_black_moneyCharacter_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
