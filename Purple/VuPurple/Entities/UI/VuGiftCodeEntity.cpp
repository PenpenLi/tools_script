//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  gift code entity
// 
//*****************************************************************************

#include "VuPurple/Managers/VuGiftManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Entities/UI/VuUITextBaseEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuBasicProperty.h"
#include "VuEngine/HAL/Keyboard/VuKeyboard.h"


class VuGiftCodeEntity : public VuUITextBaseEntity
{
	DECLARE_RTTI

public:
	VuGiftCodeEntity();

protected:
	// event handlers
	void				OnRedeemCodeResult(const VuParams &params);

	// scripting
	VuRetVal			Key0(const VuParams &params) { pressKey(VUKEY_0); return VuRetVal(); }
	VuRetVal			Key1(const VuParams &params) { pressKey(VUKEY_1); return VuRetVal(); }
	VuRetVal			Key2(const VuParams &params) { pressKey(VUKEY_2); return VuRetVal(); }
	VuRetVal			Key3(const VuParams &params) { pressKey(VUKEY_3); return VuRetVal(); }
	VuRetVal			Key4(const VuParams &params) { pressKey(VUKEY_4); return VuRetVal(); }
	VuRetVal			Key5(const VuParams &params) { pressKey(VUKEY_5); return VuRetVal(); }
	VuRetVal			Key6(const VuParams &params) { pressKey(VUKEY_6); return VuRetVal(); }
	VuRetVal			Key7(const VuParams &params) { pressKey(VUKEY_7); return VuRetVal(); }
	VuRetVal			Key8(const VuParams &params) { pressKey(VUKEY_8); return VuRetVal(); }
	VuRetVal			Key9(const VuParams &params) { pressKey(VUKEY_9); return VuRetVal(); }
	VuRetVal			KeyBack(const VuParams &params) { pressKey(VUKEY_BACK); return VuRetVal(); }
	VuRetVal			KeyEnter(const VuParams &params) { pressKey(VUKEY_ENTER); return VuRetVal(); }

	void				pressKey(VUUINT32 key);

	virtual const char	*getText() { return mText.c_str(); }

	std::string			mText;
};

IMPLEMENT_RTTI(VuGiftCodeEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuGiftCodeEntity);


//*****************************************************************************
VuGiftCodeEntity::VuGiftCodeEntity()
{
	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGiftCodeEntity, Key0);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGiftCodeEntity, Key1);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGiftCodeEntity, Key2);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGiftCodeEntity, Key3);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGiftCodeEntity, Key4);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGiftCodeEntity, Key5);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGiftCodeEntity, Key6);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGiftCodeEntity, Key7);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGiftCodeEntity, Key8);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGiftCodeEntity, Key9);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGiftCodeEntity, KeyBack);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuGiftCodeEntity, KeyEnter);

	// event handlers
	REG_EVENT_HANDLER(VuGiftCodeEntity, OnRedeemCodeResult);
}

//*****************************************************************************
void VuGiftCodeEntity::OnRedeemCodeResult(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	bool success = accessor.getBool();

	if ( success )
		mText.clear();
}

//*****************************************************************************
void VuGiftCodeEntity::pressKey(VUUINT32 key)
{
	if ( key >= VUKEY_0 && key <= VUKEY_9 )
	{
		if ( (int)mText.length() < VuGiftManager::CODE_LENGTH )
			mText.push_back('0' + (key - VUKEY_0));

		VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
	}
	else if ( key == VUKEY_BACK )
	{
		if ( mText.length() > 0 )
			mText.resize(mText.size() - 1);

		VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_BACK);
	}
	else if ( key == VUKEY_ENTER )
	{
		VuGiftManager::IF()->redeemCode(mText);

		VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_SELECT);
	}
}
