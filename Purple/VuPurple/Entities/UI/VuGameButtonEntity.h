//*****************************************************************************
//
//  Copyright (c) 2009-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game Button class
// 
//*****************************************************************************

#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"

class VuScriptComponent;
class VuTransitionComponent;
struct VuUIDrawParams;
class VuFont;

class VuGameButtonBaseEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuGameButtonBaseEntity();

	virtual void	onGameInitialize();


	enum GameButtonType
	{
		BTN_TYPE_COMMON,
		BTN_TYPE_BIG_GIFT,
	
	};

protected:
	// scripting
	VuRetVal						SetFocus(const VuParams &params = VuParams());
	VuRetVal						SetStringID(const VuParams &params);

// modify by xlm,[/2014/12/2 ] 
	VuRetVal						SetRmbTextId(const VuParams &params);
	VuRetVal						Show(const VuParams &params);
	VuRetVal						Hide(const VuParams &params);
	VuRetVal						IsVisible(const VuParams &params);
	VuRetVal						Enable(const VuParams &params);
	VuRetVal						Disable(const VuParams &params);
	VuRetVal						IsEnabled(const VuParams &params);

	// event handlers
	virtual void							OnUITick(const VuParams &params);  // modify by xlm,[/2014/12/22 ] 
	void							OnUIGamePad(const VuParams &params);
	void							OnUITouch(const VuParams &params);
	void							OnUIDraw(const VuParams &params);

	void							drawLayout(bool bSelected);
	void							drawButton(float alpha);

	//modify by xlm
	virtual void							drawBaseText(int nDepth, VuFont* pFont, const char *strText ,VuFontDrawParams& fontParam, VuRect& rect, VUUINT32 uFlag, float alpha);

	virtual void					drawIcon(float depth, const VuColor &color, const VuRect &dstRect, const VuRect &srcRect) = 0;
	virtual void					drawSecondaryIcon(float depth, const VuColor &color, const VuUIDrawParams &uiDrawParams) {}

	bool							isHidden() { return mHidden; }
	bool							isVisible() { return !mHidden; }

	bool							isDisabled() { return mDisabled; }
	bool							isEnabled() { return !mDisabled; }

	bool							isVisibleAndActive();

	// components
	VuScriptComponent				*mpScriptComponent;
	VuTransitionComponent			*mpTransitionComponent;

	// properties
	bool							mInitiallyHidden;
	bool							mDisabled;
	VuUIRectProperties				mImageRect;
	VuUIRectProperties				mTouchRect;
	VuUIRectProperties				mTextRect;
	VuUIRectProperties				mIconRect;

// modify by xlm,[/2014/12/2 ] 按钮添加一个rmb价格
	VuUIRectProperties				mRmbTextRect;
	std::string						mRmbTextId;  //rmb价格


	/*int								mButtonType;*/


	std::string						mFont;
	std::string						mStringID;
	std::string						mGamePadStringID;
	VuUIImageProperties				mSelectedTexture;
	VuUIImageProperties				mUnselectedTexture;
	bool							mFlipImageX;
	bool							mFlipImageY;
	bool							mFlipIconX;
	bool							mFlipIconY;
	VuUIAnchorProperties			mAnchor;
	bool							mCanHaveFocus;
	bool							mSwapIconDepth;

	bool							mHidden;

	bool							mNewTouch;
	bool							mTouchDown;
	bool							mTouchUp;
	VuVector2						mTouchPos;

	enum ePressedState { NOT_PRESSED, PRESSED };
	ePressedState					mPressedState;
	float							mPressedScale;
};
