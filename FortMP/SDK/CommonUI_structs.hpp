#pragma once

// Dumped with Dumper-7!


namespace SDK
{
//---------------------------------------------------------------------------------------------------------------------
// ENUMS
//---------------------------------------------------------------------------------------------------------------------

enum class ECommonNumericType : uint8
{
	Number                         = 0,
	Percentage                     = 1,
	Seconds                        = 2,
	Distance                       = 3,
	ECommonNumericType_MAX         = 4,
};

enum class EOperation : uint8
{
	Intro                          = 0,
	Outro                          = 1,
	Push                           = 2,
	Pop                            = 3,
	Invalid                        = 4,
	EOperation_MAX                 = 5,
};

enum class ERichTextInlineIconDisplayMode : uint8
{
	IconOnly                       = 0,
	TextOnly                       = 1,
	IconAndText                    = 2,
	MAX                            = 3,
};

enum class ECommonInputMode : uint8
{
	Menu                           = 0,
	Game                           = 1,
	All                            = 2,
	MAX                            = 3,
};

enum class EInputActionState : uint8
{
	Enabled                        = 0,
	Disabled                       = 1,
	Hidden                         = 2,
	HiddenAndDisabled              = 3,
	EInputActionState_MAX          = 4,
};

enum class ETransitionCurve : uint8
{
	Linear                         = 0,
	QuadIn                         = 1,
	QuadOut                        = 2,
	QuadInOut                      = 3,
	CubicIn                        = 4,
	CubicOut                       = 5,
	CubicInOut                     = 6,
	ETransitionCurve_MAX           = 7,
};

enum class ECommonSwitcherTransition : uint8
{
	FadeOnly                       = 0,
	Horizontal                     = 1,
	Vertical                       = 2,
	Zoom                           = 3,
	ECommonSwitcherTransition_MAX  = 4,
};


//---------------------------------------------------------------------------------------------------------------------
// STRUCTS
//---------------------------------------------------------------------------------------------------------------------

// 0x14 (0x14 - 0x0)
// ScriptStruct CommonUI.CommonNumberFormattingOptions
struct FCommonNumberFormattingOptions
{
public:
	enum class ERoundingMode                     RoundingMode;                                      // 0x0(0x1)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                         UseGrouping;                                       // 0x1(0x1)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                        Pad_B6A[0x2];                                      // Fixing Size After Last Property  [ Dumper-7 ]
	int32                                        MinimumIntegralDigits;                             // 0x4(0x4)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32                                        MaximumIntegralDigits;                             // 0x8(0x4)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32                                        MinimumFractionalDigits;                           // 0xC(0x4)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	int32                                        MaximumFractionalDigits;                           // 0x10(0x4)(Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

// 0x18 (0x18 - 0x0)
// ScriptStruct CommonUI.CommonRegisteredTabInfo
struct FCommonRegisteredTabInfo
{
public:
	int32                                        TabIndex;                                          // 0x0(0x4)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                        Pad_B71[0x4];                                      // Fixing Size After Last Property  [ Dumper-7 ]
	class UCommonButton*                         TabButton;                                         // 0x8(0x8)(ExportObject, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class UWidget*                               ContentInstance;                                   // 0x10(0x8)(ExportObject, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

// 0x20 (0x20 - 0x0)
// ScriptStruct CommonUI.CommonInputActionHandlerData
struct FCommonInputActionHandlerData
{
public:
	struct FDataTableRowHandle                   InputActionRow;                                    // 0x0(0x10)(Edit, BlueprintVisible, BlueprintReadOnly, NoDestructor, NativeAccessSpecifierPublic)
	enum class EInputActionState                 State;                                             // 0x10(0x1)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
	uint8                                        Pad_B79[0xF];                                      // Fixing Size Of Struct [ Dumper-7 ]
};

// 0x20 (0x20 - 0x0)
// ScriptStruct CommonUI.CommonButtonStyleOptionalSlateSound
struct FCommonButtonStyleOptionalSlateSound
{
public:
	bool                                         bHasSound;                                         // 0x0(0x1)(Edit, BlueprintVisible, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                        Pad_B7A[0x7];                                      // Fixing Size After Last Property  [ Dumper-7 ]
	struct FSlateSound                           Sound;                                             // 0x8(0x18)(Edit, BlueprintVisible, NoDestructor, NativeAccessSpecifierPublic)
};

// 0x18 (0x18 - 0x0)
// ScriptStruct CommonUI.Operation
struct FOperation
{
public:
	enum class EOperation                        Operation;                                         // 0x0(0x1)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                        Pad_B7D[0x7];                                      // Fixing Size After Last Property  [ Dumper-7 ]
	class UCommonActivatablePanel*               Panel;                                             // 0x8(0x8)(ExportObject, ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                         bIntroPanel;                                       // 0x10(0x1)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                         bActivatePanel;                                    // 0x11(0x1)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                         bOutroPanelBelow;                                  // 0x12(0x1)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                        Pad_B7E[0x5];                                      // Fixing Size Of Struct [ Dumper-7 ]
};

// 0x24 (0x24 - 0x0)
// ScriptStruct CommonUI.CommonAnalogCursorSettings
struct FCommonAnalogCursorSettings
{
public:
	int32                                        PreprocessorPriority;                              // 0x0(0x4)(Edit, ZeroConstructor, Config, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                         bEnableCursorAcceleration;                         // 0x4(0x1)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                        Pad_B82[0x3];                                      // Fixing Size After Last Property  [ Dumper-7 ]
	float                                        CursorAcceleration;                                // 0x8(0x4)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                        CursorMaxSpeed;                                    // 0xC(0x4)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                        CursorDeadZone;                                    // 0x10(0x4)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                        HoverSlowdownFactor;                               // 0x14(0x4)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                        ScrollDeadZone;                                    // 0x18(0x4)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                        ScrollUpdatePeriod;                                // 0x1C(0x4)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                        ScrollMultiplier;                                  // 0x20(0x4)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

// 0x0 (0x8 - 0x8)
// ScriptStruct CommonUI.UITag
struct FUITag : public FGameplayTag
{
public:
};

// 0x0 (0x8 - 0x8)
// ScriptStruct CommonUI.UIActionTag
struct FUIActionTag : public FUITag
{
public:
};

// 0x20 (0x20 - 0x0)
// ScriptStruct CommonUI.UIActionKeyMapping
struct FUIActionKeyMapping
{
public:
	struct FKey                                  Key;                                               // 0x0(0x18)(Edit, Config, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                        HoldTime;                                          // 0x18(0x4)(Edit, ZeroConstructor, Config, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                        Pad_B89[0x4];                                      // Fixing Size Of Struct [ Dumper-7 ]
};

// 0x30 (0x30 - 0x0)
// ScriptStruct CommonUI.UIInputAction
struct FUIInputAction
{
public:
	struct FUIActionTag                          ActionTag;                                         // 0x0(0x8)(Edit, Config, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	class FText                                  DefaultDisplayName;                                // 0x8(0x18)(Edit, Config, NativeAccessSpecifierPublic)
	TArray<struct FUIActionKeyMapping>           KeyMappings;                                       // 0x20(0x10)(Edit, ZeroConstructor, Config, NativeAccessSpecifierPublic)
};

// 0x48 (0x50 - 0x8)
// ScriptStruct CommonUI.RichTextIconData
struct FRichTextIconData : public FTableRowBase
{
public:
	class FText                                  DisplayName;                                       // 0x8(0x18)(Edit, DisableEditOnInstance, NativeAccessSpecifierPublic)
	TSoftObjectPtr<class UObject>                ResourceObject;                                    // 0x20(0x28)(Edit, DisableEditOnInstance, UObjectWrapper, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FVector2D                             ImageSize;                                         // 0x48(0x8)(Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
};

// 0xA8 (0xA8 - 0x0)
// ScriptStruct CommonUI.CommonInputTypeInfo
struct FCommonInputTypeInfo
{
public:
	struct FKey                                  Key;                                               // 0x0(0x18)(Edit, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
	enum class EInputActionState                 OverrrideState;                                    // 0x18(0x1)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	bool                                         bActionRequiresHold;                               // 0x19(0x1)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	uint8                                        Pad_B8F[0x2];                                      // Fixing Size After Last Property  [ Dumper-7 ]
	float                                        HoldTime;                                          // 0x1C(0x4)(Edit, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	struct FSlateBrush                           OverrideBrush;                                     // 0x20(0x88)(Edit, NativeAccessSpecifierPublic)
};

// 0x420 (0x428 - 0x8)
// ScriptStruct CommonUI.CommonInputActionData
struct FCommonInputActionData : public FTableRowBase
{
public:
	class FText                                  DisplayName;                                       // 0x8(0x18)(Edit, BlueprintVisible, BlueprintReadOnly, NativeAccessSpecifierPublic)
	class FText                                  HoldDisplayName;                                   // 0x20(0x18)(Edit, BlueprintVisible, BlueprintReadOnly, NativeAccessSpecifierPublic)
	struct FCommonInputTypeInfo                  KeyboardInputTypeInfo;                             // 0x38(0xA8)(Edit, Protected, NativeAccessSpecifierProtected)
	struct FCommonInputTypeInfo                  GamepadInputTypeInfos[0x4];                        // 0xE0(0x2A0)(Edit, Protected, NativeAccessSpecifierProtected)
	struct FCommonInputTypeInfo                  TouchInputTypeInfo;                                // 0x380(0xA8)(Edit, Protected, NativeAccessSpecifierProtected)
};

}


