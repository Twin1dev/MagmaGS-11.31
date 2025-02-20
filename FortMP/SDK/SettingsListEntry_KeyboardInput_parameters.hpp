#pragma once

// Dumped with Dumper-7!


#include "../SDK.hpp"

namespace SDK
{
namespace Params
{
//---------------------------------------------------------------------------------------------------------------------
// PARAMETERS
//---------------------------------------------------------------------------------------------------------------------

// 0x30 (0x30 - 0x0)
// Function SettingsListEntry_KeyboardInput.SettingsListEntry_KeyboardInput_C.OnUpdateKeyText
struct USettingsListEntry_KeyboardInput_C_OnUpdateKeyText_Params
{
public:
	class FText                                  PrimaryKeyText;                                    // 0x0(0x18)(ConstParm, BlueprintVisible, BlueprintReadOnly, Parm, OutParm, ReferenceParm)
	class FText                                  SecondaryKeyText;                                  // 0x18(0x18)(ConstParm, BlueprintVisible, BlueprintReadOnly, Parm, OutParm, ReferenceParm)
};

// 0xA8 (0xA8 - 0x0)
// Function SettingsListEntry_KeyboardInput.SettingsListEntry_KeyboardInput_C.OnMouseEnter
struct USettingsListEntry_KeyboardInput_C_OnMouseEnter_Params
{
public:
	struct FGeometry                             MyGeometry;                                        // 0x0(0x38)(BlueprintVisible, BlueprintReadOnly, Parm, IsPlainOldData, NoDestructor)
	struct FPointerEvent                         MouseEvent;                                        // 0x38(0x70)(ConstParm, BlueprintVisible, BlueprintReadOnly, Parm, OutParm, ReferenceParm)
};

// 0x70 (0x70 - 0x0)
// Function SettingsListEntry_KeyboardInput.SettingsListEntry_KeyboardInput_C.OnMouseLeave
struct USettingsListEntry_KeyboardInput_C_OnMouseLeave_Params
{
public:
	struct FPointerEvent                         MouseEvent;                                        // 0x0(0x70)(ConstParm, BlueprintVisible, BlueprintReadOnly, Parm, OutParm, ReferenceParm)
};

// 0x180 (0x180 - 0x0)
// Function SettingsListEntry_KeyboardInput.SettingsListEntry_KeyboardInput_C.ExecuteUbergraph_SettingsListEntry_KeyboardInput
struct USettingsListEntry_KeyboardInput_C_ExecuteUbergraph_SettingsListEntry_KeyboardInput_Params
{
public:
	int32                                        EntryPoint;                                        // 0x0(0x4)(BlueprintVisible, BlueprintReadOnly, Parm, ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash)
	uint8                                        Pad_618C[0x4];                                     // Fixing Size After Last Property  [ Dumper-7 ]
	struct FPointerEvent                         K2Node_Event_MouseEvent;                           // 0x8(0x70)(ConstParm)
	class FText                                  K2Node_Event_PrimaryKeyText;                       // 0x78(0x18)(ConstParm)
	class FText                                  K2Node_Event_SecondaryKeyText;                     // 0x90(0x18)(ConstParm)
	class UScrollingTextButton_C*                K2Node_DynamicCast_AsScrolling_Text_Button;        // 0xA8(0x8)(ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash)
	bool                                         K2Node_DynamicCast_bSuccess;                       // 0xB0(0x1)(ZeroConstructor, IsPlainOldData, NoDestructor)
	uint8                                        Pad_618D[0x7];                                     // Fixing Size After Last Property  [ Dumper-7 ]
	class UScrollingTextButton_C*                K2Node_DynamicCast_AsScrolling_Text_Button_1;      // 0xB8(0x8)(ZeroConstructor, InstancedReference, IsPlainOldData, NoDestructor, HasGetValueTypeHash)
	bool                                         K2Node_DynamicCast_bSuccess_1;                     // 0xC0(0x1)(ZeroConstructor, IsPlainOldData, NoDestructor)
	uint8                                        Pad_618E[0x3];                                     // Fixing Size After Last Property  [ Dumper-7 ]
	struct FGeometry                             K2Node_Event_MyGeometry;                           // 0xC4(0x38)(IsPlainOldData, NoDestructor)
	uint8                                        Pad_618F[0x4];                                     // Fixing Size After Last Property  [ Dumper-7 ]
	struct FPointerEvent                         K2Node_Event_MouseEvent_1;                         // 0x100(0x70)(ConstParm)
	class UUMGSequencePlayer*                    CallFunc_PlayAnimationReverse_ReturnValue;         // 0x170(0x8)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash)
	class UUMGSequencePlayer*                    CallFunc_PlayAnimationForward_ReturnValue;         // 0x178(0x8)(ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash)
};

}
}


