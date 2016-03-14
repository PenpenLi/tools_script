//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Tuning entities
// 
//*****************************************************************************

#include "VuPurple/Managers/VuGameManager.h"
#include "VuPurple/Managers/VuCloudTuningManager.h"
#include "VuPurple/Util/VuGameUtil.h"
#include "VuPurple/Util/VuGameFontMacros.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Properties/VuDBEntryProperty.h"
#include "VuEngine/Util/VuStringUtil.h"


//*****************************************************************************
// VuTuningBooleanEntity
//*****************************************************************************
class VuTuningBooleanEntity : public VuEntity
{
	DECLARE_RTTI
public:
	VuTuningBooleanEntity();

private:
	VuRetVal			Test(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mVariable;
};

IMPLEMENT_RTTI(VuTuningBooleanEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuTuningBooleanEntity);


//*****************************************************************************
VuTuningBooleanEntity::VuTuningBooleanEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	addProperty(new VuDBEntryProperty("Variable", mVariable, "TuningDB"));

	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuTuningBooleanEntity, Test);

	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnTrue);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFalse);
}

//*****************************************************************************
VuRetVal VuTuningBooleanEntity::Test(const VuParams &params)
{
	if ( VuCloudTuningManager::IF()->tuningDB()[mVariable].asBool() )
		mpScriptComponent->getPlug("OnTrue")->execute();
	else
		mpScriptComponent->getPlug("OnFalse")->execute();

	return VuRetVal();
}


//*****************************************************************************
// VuTuningIntegerEntity
//*****************************************************************************
class VuTuningIntegerEntity : public VuEntity
{
	DECLARE_RTTI
public:
	VuTuningIntegerEntity();

private:
	VuRetVal			Get(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mVariable;
};

IMPLEMENT_RTTI(VuTuningIntegerEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuTuningIntegerEntity);


//*****************************************************************************
VuTuningIntegerEntity::VuTuningIntegerEntity()
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	addProperty(new VuDBEntryProperty("Variable", mVariable, "TuningDB"));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuTuningIntegerEntity, Get, VuRetVal::Int, VuParamDecl());
}

//*****************************************************************************
VuRetVal VuTuningIntegerEntity::Get(const VuParams &params)
{
	int value = VuCloudTuningManager::IF()->tuningDB()[mVariable].asInt();

	return VuRetVal(value);
}


//*****************************************************************************
// VuTuningPriceEntity
//*****************************************************************************
class VuTuningPriceEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuTuningPriceEntity();

	virtual void		onGameInitialize();

private:
	VuRetVal			IsSC(const VuParams &params) { return VuRetVal(mSC > 0); }
	VuRetVal			IsPC(const VuParams &params) { return VuRetVal(mPC > 0); }
	VuRetVal			IsVideo(const VuParams &params) { return VuRetVal(mVideo); }
	VuRetVal			CanAfford(const VuParams &params);
	VuRetVal			Purchase(const VuParams &params);

	// components
	VuScriptComponent	*mpScriptComponent;

	// properties
	std::string			mVariable;

	int					mSC;
	int					mPC;
	bool				mVideo;
};

IMPLEMENT_RTTI(VuTuningPriceEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuTuningPriceEntity);


//*****************************************************************************
VuTuningPriceEntity::VuTuningPriceEntity():
	mSC(0),
	mPC(0),
	mVideo(false)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));

	// properties
	addProperty(new VuDBEntryProperty("Variable", mVariable, "TuningDB"));

	ADD_SCRIPT_INPUT(mpScriptComponent, VuTuningPriceEntity, IsSC, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuTuningPriceEntity, IsPC, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuTuningPriceEntity, IsVideo, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT(mpScriptComponent, VuTuningPriceEntity, CanAfford, VuRetVal::Bool, VuParamDecl());
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuTuningPriceEntity, Purchase);

	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnSuccess);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFailure);
}

//*****************************************************************************
void VuTuningPriceEntity::onGameInitialize()
{
	const VuJsonContainer &data = VuCloudTuningManager::IF()->tuningDB()[mVariable];
	mSC = data["SC"].asInt();
	mPC = data["PC"].asInt();
	mVideo = data["Video"].asBool();

	char str[64];
	VuStringUtil::integerFormat(mSC + mPC, str, sizeof(str));

	std::string macro = "TUNING_PRICE_" + mVariable;
	VuGameFontMacros::IF()->setMacro(macro.c_str(), str);
}

//*****************************************************************************
VuRetVal VuTuningPriceEntity::CanAfford(const VuParams &params)
{
	bool canAfford = VuGameManager::IF()->canAfford(mSC, mPC);

	return VuRetVal(canAfford);
}

//*****************************************************************************
VuRetVal VuTuningPriceEntity::Purchase(const VuParams &params)
{
	if ( VuGameManager::IF()->spendCurrency(mSC, mPC) )
		mpScriptComponent->getPlug("OnSuccess")->execute();
	else
		mpScriptComponent->getPlug("OnFailure")->execute();

	return VuRetVal();
}
