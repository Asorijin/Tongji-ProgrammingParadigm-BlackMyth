// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "UIBasePanel.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef BLACK_MONEY_UIBasePanel_generated_h
#error "UIBasePanel.generated.h already included, missing '#pragma once' in UIBasePanel.h"
#endif
#define BLACK_MONEY_UIBasePanel_generated_h

#define FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_UIBasePanel_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execHidePanel); \
	DECLARE_FUNCTION(execShowPanel);


#define FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_UIBasePanel_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUUIBasePanel(); \
	friend struct Z_Construct_UClass_UUIBasePanel_Statics; \
public: \
	DECLARE_CLASS(UUIBasePanel, UUserWidget, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/black_money"), NO_API) \
	DECLARE_SERIALIZER(UUIBasePanel)


#define FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_UIBasePanel_h_15_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UUIBasePanel(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UUIBasePanel(UUIBasePanel&&); \
	UUIBasePanel(const UUIBasePanel&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UUIBasePanel); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UUIBasePanel); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UUIBasePanel) \
	NO_API virtual ~UUIBasePanel();


#define FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_UIBasePanel_h_12_PROLOG
#define FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_UIBasePanel_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_UIBasePanel_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_UIBasePanel_h_15_INCLASS_NO_PURE_DECLS \
	FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_UIBasePanel_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> BLACK_MONEY_API UClass* StaticClass<class UUIBasePanel>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_Tongji_ProgrammingParadigm_BlackMyth_Source_black_money_Public_UIBasePanel_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
