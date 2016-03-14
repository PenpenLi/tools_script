//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Color Wheel Entity
// 
//*****************************************************************************

#include "VuPurple/Util/VuGameUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"
#include "VuEngine/Components/2dLayout/Vu2dLayoutComponent.h"
#include "VuEngine/Components/VuTransitionComponent.h"
#include "VuEngine/Properties/VuAssetProperty.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Events/VuEventManager.h"
#include "VuEngine/HAL/Touch/VuTouch.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/UI/VuUIDrawUtil.h"
#include "VuEngine/UI/VuUIInputUtil.h"
#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Gfx/Shaders/VuBasicShaders.h"
#include "VuEngine/UI/VuUI.h"
#include "VuEngine/Memory/VuScratchPad.h"
#include "VuEngine/Math/VuMathUtil.h"


class VuWheelEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuWheelEntity();

protected:
	virtual int			getItemCount() = 0;
	virtual VuColor		getItemColor(int index) = 0;
	virtual VuTexture	*getItemTexture(int index) = 0;
	virtual int			getIndex() = 0;
	virtual void		setIndex(int index) = 0;

	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUIGamePad(const VuParams &params);
	void				OnUITouch(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	// scripting
	VuRetVal			SetFocus(const VuParams &params = VuParams());

	void				drawLayout(bool bSelected) { drawWheel(1.0f); }
	void				drawWheel(float alpha);

	int					getNormalizedSlice(int slice);
	bool				isSliceVisible(const VuVector2 &center, float innerRadius, float outerRadius, float minAngle, float maxAngle);
	void				drawSlice(float angle, const VuColor &color, VuTexture *pTexture);

	// components
	VuScriptComponent		*mpScriptComponent;
	VuTransitionComponent	*mpTransitionComponent;

	// properties
	float					mAngularOffset;
	float					mInnerRadius;
	float					mOuterRadius;
	float					mWedgeAngle;
	float					mSpaceAngle;
	float					mSubdivisionAngle;
	float					mRotationLag;
	VuUIAnchorProperties	mAnchor;
	VuVector2				mTexCoord0;
	VuVector2				mTexCoord1;

	float					mCurAngle;
	float					mAngularVel;

	bool					mNewTouch;
	bool					mTouchDown;
	bool					mTouchUp;
	VuVector2				mTouchPos;

	enum eState { WAITING, SCROLLING };
	eState					mState;
	VuVector2				mInitialTouchDown;
	float					mInitialTouchAngle;
};


IMPLEMENT_RTTI(VuWheelEntity, VuEntity);


//*****************************************************************************
VuWheelEntity::VuWheelEntity():
	mAngularOffset(0),
	mInnerRadius(0),
	mOuterRadius(0),
	mWedgeAngle(15.0f),
	mSpaceAngle(0.0f),
	mSubdivisionAngle(1.0f),
	mRotationLag(0.1f),
	mTexCoord0(0,0),
	mTexCoord1(1,1),
	mCurAngle(0.0f),
	mAngularVel(0.0f),
	mNewTouch(false),
	mTouchDown(false),
	mTouchUp(false),
	mState(WAITING)
{
	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this));
	addComponent(new Vu2dLayoutComponent(this, &VuWheelEntity::drawLayout));
	addComponent(mpTransitionComponent = new VuTransitionComponent(this));

	// properties
	addProperty(new VuFloatProperty("Angular Offset", mAngularOffset));
	addProperty(new VuFloatProperty("Inner Radius", mInnerRadius));
	addProperty(new VuFloatProperty("Outer Radius", mOuterRadius));
	addProperty(new VuFloatProperty("Wedge Angle", mWedgeAngle));
	addProperty(new VuFloatProperty("Space Angle", mSpaceAngle));
	addProperty(new VuFloatProperty("Subdivision Angle", mSubdivisionAngle));
	addProperty(new VuFloatProperty("Rotation Lag", mRotationLag));
	ADD_UI_ANCHOR_PROPERTIES(getProperties(), mAnchor, "");
	addProperty(new VuVector2Property("Tex Coord 0", mTexCoord0));
	addProperty(new VuVector2Property("Tex Coord 1", mTexCoord1));

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuWheelEntity, SetFocus);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavLeft);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, NavRight);
	ADD_SCRIPT_OUTPUT_NOARGS(mpScriptComponent, OnFocus);

	// event handlers
	REG_EVENT_HANDLER(VuWheelEntity, OnUITick);
	REG_EVENT_HANDLER(VuWheelEntity, OnUIGamePad);
	REG_EVENT_HANDLER(VuWheelEntity, OnUITouch);
	REG_EVENT_HANDLER(VuWheelEntity, OnUIDraw);
}

//*****************************************************************************
void VuWheelEntity::OnUITick(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	float fdt = accessor.getFloat();

	mpTransitionComponent->tick(fdt);

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	if ( mNewTouch )
	{
		mInitialTouchDown = mTouchPos;
		mInitialTouchAngle = mCurAngle;

		VuVector2 touchPolar = VuMathUtil::cartesianToPolar(mTouchPos/uiDrawParams.mInvAuthScale - uiDrawParams.mPosition);
		touchPolar.mY = VuRadiansToDegrees(touchPolar.mY) + mAngularOffset;
		touchPolar.mY = VuRadiansToDegrees(VuAngClamp(VuDegreesToRadians(touchPolar.mY)));
		if ( touchPolar.mX >= mInnerRadius && touchPolar.mX <= mOuterRadius )
		{
			SetFocus();
			mState = SCROLLING;
		}
	}

	if ( mState == SCROLLING )
	{
		if ( mTouchDown )
		{
			VuVector2 initialTouchPolar = VuMathUtil::cartesianToPolar(mInitialTouchDown/uiDrawParams.mInvAuthScale - uiDrawParams.mPosition);
			initialTouchPolar.mY = VuRadiansToDegrees(initialTouchPolar.mY) + mAngularOffset;
			initialTouchPolar.mY = VuRadiansToDegrees(VuAngClamp(VuDegreesToRadians(initialTouchPolar.mY)));

			VuVector2 touchPolar = VuMathUtil::cartesianToPolar(mTouchPos/uiDrawParams.mInvAuthScale - uiDrawParams.mPosition);
			touchPolar.mY = VuRadiansToDegrees(touchPolar.mY) + mAngularOffset;
			touchPolar.mY = VuRadiansToDegrees(VuAngClamp(VuDegreesToRadians(touchPolar.mY)));

			float targetAngle = mInitialTouchAngle + (touchPolar.mY - initialTouchPolar.mY);
			mCurAngle = VuMathUtil::smoothCD(mCurAngle, targetAngle, mAngularVel, mRotationLag, fdt);

			int curColorIndex = getNormalizedSlice(VuRound(mCurAngle/mWedgeAngle));
			if ( curColorIndex != getIndex() )
				setIndex(curColorIndex);
		}
		else
		{
			mState = WAITING;
			int targetSlice = getNormalizedSlice(VuRound((mCurAngle + mAngularVel*0.25f)/mWedgeAngle));
			if ( targetSlice != getIndex() )
				setIndex(targetSlice);
		}
	}
	else
	{
		// chase current color
		float targetAngle = getIndex()*mWedgeAngle;
		float totalRange = getItemCount()*mWedgeAngle;

		float deltaAngle = targetAngle - mCurAngle;
		deltaAngle += 0.5f*totalRange;
		deltaAngle /= totalRange;
		deltaAngle -= VuFloor(deltaAngle);
		deltaAngle *= totalRange;
		deltaAngle -= 0.5f*totalRange;
		targetAngle = mCurAngle + deltaAngle;

		mCurAngle = VuMathUtil::smoothCD(mCurAngle, targetAngle, mAngularVel, mRotationLag, fdt);
	}

	if ( mTouchUp )
	{
		mTouchDown = false;
		mTouchUp = false;
	}
	mNewTouch = false;
}

//*****************************************************************************
void VuWheelEntity::OnUIGamePad(const VuParams &params)
{
	if ( VuUI::IF()->getFocus() == this )
	{
		VuParams::VuAccessor accessor(params);
		int channel = accessor.getInt();
		bool down = accessor.getBool();
		int padIndex = accessor.getInt();

		if ( down )
		{
			if ( channel == VuUIInputUtil::CHANNEL_UP )
			{
				int itemIndex = getIndex();
				itemIndex = (itemIndex + 1)%getItemCount();
				setIndex(itemIndex);

				VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
			}
			else if ( channel == VuUIInputUtil::CHANNEL_DOWN )
			{
				int itemIndex = getIndex();
				itemIndex = (itemIndex - 1 + getItemCount()) % (int)getItemCount();
				setIndex(itemIndex);

				VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
			}
			else if ( channel == VuUIInputUtil::CHANNEL_LEFT )
			{
				mpScriptComponent->getPlug("NavLeft")->execute();

				if ( mpScriptComponent->getPlug("NavLeft")->getNumConnections() )
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
			}
			else if ( channel == VuUIInputUtil::CHANNEL_RIGHT )
			{
				mpScriptComponent->getPlug("NavRight")->execute();

				if ( mpScriptComponent->getPlug("NavRight")->getNumConnections() )
					VuGameUtil::IF()->playSfx(VuGameUtil::SFX_UI_NAVIGATE);
			}
		}
	}
}

//*****************************************************************************
void VuWheelEntity::OnUITouch(const VuParams &params)
{
	VuParams::VuAccessor accessor(params);
	int action = accessor.getInt();
	VuVector2 touch = accessor.getVector2();
		
	if ( action == VuUIInputUtil::TOUCH_DOWN )
	{
		if ( !mTouchDown )
		{
			mTouchDown = true;
			mNewTouch = true;
			mAnchor.unapply(touch, mTouchPos);
		}
	}
	else if ( action == VuUIInputUtil::TOUCH_UP )
	{
		mTouchUp = true;
	}
	else if ( action == VuUIInputUtil::TOUCH_MOVE )
	{
		mAnchor.unapply(touch, mTouchPos);
	}
}

//*****************************************************************************
void VuWheelEntity::OnUIDraw(const VuParams &params)
{
	drawWheel(mpTransitionComponent->alpha());
}

//*****************************************************************************
VuRetVal VuWheelEntity::SetFocus(const VuParams &params)
{
	VuUI::IF()->setFocus(this);
	mpScriptComponent->getPlug("OnFocus")->execute();

	return VuRetVal();
}

//*****************************************************************************
void VuWheelEntity::drawWheel(float alpha)
{
	int curSlice = VuRound(mCurAngle/mWedgeAngle);

	int sliceCount = VuTruncate(360.0f/mWedgeAngle)/2 - 1;
	for ( int i = -sliceCount; i <= sliceCount; i++ )
	{
		int itemIndex = getNormalizedSlice(curSlice - i);
		VuColor color = getItemColor(itemIndex);
		color.mA = VuRound(color.mA*alpha);
		VuTexture *pTexture = getItemTexture(itemIndex);

		drawSlice((i - curSlice)*mWedgeAngle + mCurAngle + mAngularOffset, color, pTexture);
	}
}

//*****************************************************************************
int VuWheelEntity::getNormalizedSlice(int slice)
{
	if ( slice < 0 )
		slice += (1 - slice/(int)getItemCount()) * (int)getItemCount();

	return slice%getItemCount();
}

//*****************************************************************************
bool VuWheelEntity::isSliceVisible(const VuVector2 &center, float innerRadius, float outerRadius, float minAngle, float maxAngle)
{
	float minAngleSin, minAngleCos;
	VuSinCos(VuDegreesToRadians(minAngle), minAngleSin, minAngleCos);

	float maxAngleSin, maxAngleCos;
	VuSinCos(VuDegreesToRadians(maxAngle), maxAngleSin, maxAngleCos);

	VuVector2 v[4];
	v[0] = VuVector2(innerRadius*minAngleCos, innerRadius*minAngleSin);
	v[1] = VuVector2(outerRadius*minAngleCos, outerRadius*minAngleSin);
	v[2] = VuVector2(innerRadius*maxAngleCos, innerRadius*maxAngleSin);
	v[3] = VuVector2(outerRadius*maxAngleCos, outerRadius*maxAngleSin);

	float minX = FLT_MAX, minY = FLT_MAX, maxX = -FLT_MAX, maxY = -FLT_MAX;
	for ( int i = 0; i < 4; i++ )
	{
		VuVector2 vert = (center + v[i])/VuUI::IF()->getAuthoringScreenScale();
		vert= VuGfxUtil::IF()->getMatrix().transform(vert);
		minX = VuMin(minX, vert.mX);
		minY = VuMin(minY, vert.mY);
		maxX = VuMax(maxX, vert.mX);
		maxY = VuMax(maxY, vert.mY);
	}

	if ( minX > 1.0f ) return false;
	if ( maxX < 0.0f ) return false;
	if ( minY > 1.0f ) return false;
	if ( maxY < 0.0f ) return false;

	return true;
}

//*****************************************************************************
void VuWheelEntity::drawSlice(float angle, const VuColor &color, VuTexture *pTexture)
{
	struct DrawData
	{
		static void callback(void *data)
		{
			DrawData *pData = static_cast<DrawData *>(data);

			VuGfxUtil::IF()->basicShaders()->set2dXyzUvConstants(pData->mTransform, pData->mColor);
			VuGfxUtil::IF()->basicShaders()->set2dXyzUvTexture(pData->mpTexture);

			// use scratch pad to build verts
			VuVertex2dXyzUv *verts = (VuVertex2dXyzUv *)VuScratchPad::get(VuScratchPad::GRAPHICS);
			int vertCount = 0;

			VuVertex2dXyzUv *pv = verts;
			float angleStep = (pData->mMaxAngle - pData->mMinAngle)/pData->mSubdivCount;
			for ( int i = 0; i <= pData->mSubdivCount; i++ )
			{
				float angle = VuDegreesToRadians(pData->mMinAngle + (i+0)*angleStep);

				float s, c;
				VuSinCos(angle, s, c);

				float texCoordY = VuLerp(pData->mTexCoord0.mY, pData->mTexCoord1.mY, (float)i/pData->mSubdivCount);

				pv[0].mXyz[0] = (pData->mCenter.mX + pData->mOuterRadius*c)/pData->mScreenScale.mX;
				pv[0].mXyz[1] = (pData->mCenter.mY + pData->mOuterRadius*s)/pData->mScreenScale.mY;
				pv[0].mXyz[2] = 0.0f;
				pv[0].mUv[0] = pData->mTexCoord1.mX;
				pv[0].mUv[1] = texCoordY;

				pv[1].mXyz[0] = (pData->mCenter.mX + pData->mInnerRadius*c)/pData->mScreenScale.mX;
				pv[1].mXyz[1] = (pData->mCenter.mY + pData->mInnerRadius*s)/pData->mScreenScale.mY;
				pv[1].mXyz[2] = 0.0f;
				pv[1].mUv[0] = pData->mTexCoord0.mX;
				pv[1].mUv[1] = texCoordY;

				pv += 2;
				vertCount += 2;
			}

			VuGfx::IF()->drawPrimitiveUP(VUGFX_PT_TRIANGLESTRIP, vertCount - 2, verts);
		}

		VuMatrix	mTransform;
		VuVector2	mScreenScale;
		VuVector2	mCenter;
		float		mInnerRadius;
		float		mOuterRadius;
		float		mMinAngle;
		float		mMaxAngle;
		int			mSubdivCount;
		VuColor		mColor;
		VuTexture	*mpTexture;
		VuVector2	mTexCoord0;
		VuVector2	mTexCoord1;
	};

	VuUIDrawParams uiDrawParams;
	VuUIDrawUtil::getParams(this, uiDrawParams);

	VuVector2 center = uiDrawParams.mPosition*uiDrawParams.mInvAuthScale;
	mAnchor.apply(center, center);
	center /= uiDrawParams.mInvAuthScale;

	float minAngle = angle - 0.5f*(mWedgeAngle - mSpaceAngle);
	float maxAngle = angle + 0.5f*(mWedgeAngle - mSpaceAngle);

	if ( isSliceVisible(center, mInnerRadius, mOuterRadius, minAngle, maxAngle) )
	{
		DrawData *pData = static_cast<DrawData *>(VuGfxSort::IF()->allocateCommandMemory(sizeof(DrawData)));
		pData->mTransform = VuGfxUtil::IF()->getMatrix();
		pData->mScreenScale = VuUI::IF()->getAuthoringScreenScale();
		pData->mCenter = center;
		pData->mInnerRadius = mInnerRadius;
		pData->mOuterRadius = mOuterRadius;
		pData->mMinAngle = minAngle;
		pData->mMaxAngle = maxAngle;
		pData->mSubdivCount = VuRound((pData->mMaxAngle - pData->mMinAngle)/mSubdivisionAngle);
		pData->mColor = color;
		pData->mpTexture = pTexture;
		pData->mTexCoord0 = mTexCoord0;
		pData->mTexCoord1 = mTexCoord1;

		VuGfxSort::IF()->submitDrawCommand<true>(VuGfxSort::TRANS_UI_MODULATE, VuGfxUtil::IF()->basicShaders()->get2dXyzUvMaterial(VuBasicShaders::FLV_MODULATED), VUNULL, &DrawData::callback, uiDrawParams.mDepth);
	}
}


//*****************************************************************************
// VuColorWheelEntity
//*****************************************************************************

class VuColorWheelEntity : public VuWheelEntity
{
	DECLARE_RTTI

public:
	VuColorWheelEntity();

protected:
	virtual int			getItemCount();
	virtual VuColor		getItemColor(int index);
	virtual VuTexture	*getItemTexture(int index);
	virtual int			getIndex();
	virtual void		setIndex(int index);

	enum { PAINT_COLOR, DECAL_COLOR };

	// properties
	int					mType;
};

IMPLEMENT_RTTI(VuColorWheelEntity, VuWheelEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuColorWheelEntity);

//*****************************************************************************
VuColorWheelEntity::VuColorWheelEntity():
	mType(PAINT_COLOR)
{
	// properties
	static VuStaticIntEnumProperty::Choice sTypeChoices[] =
	{
		{ "Paint Color", PAINT_COLOR},
		{ "Decal Color", DECAL_COLOR},
		{ VUNULL }
	};
	addProperty(new VuStaticIntEnumProperty("Type", mType, sTypeChoices));
}

//*****************************************************************************
int VuColorWheelEntity::getItemCount()
{
	return VuGameUtil::IF()->colorDB().size();
}

//*****************************************************************************
VuColor VuColorWheelEntity::getItemColor(int index)
{
	VuColor color(255,255,255);
	VuDataUtil::getValue(VuGameUtil::IF()->colorDB()[index]["Color"], color);
	return color;
}

//*****************************************************************************
VuTexture *VuColorWheelEntity::getItemTexture(int index)
{
	return VuGfxUtil::IF()->whiteTexture();
}

//*****************************************************************************
int VuColorWheelEntity::getIndex()
{
	const char *colorName = "";

	if ( mType == PAINT_COLOR ) colorName = VuGameUtil::IF()->getCarPaintPaintColor().c_str();
	if ( mType == DECAL_COLOR ) colorName = VuGameUtil::IF()->getCarPaintDecalColor().c_str();

	for ( int i = 0; i < VuGameUtil::IF()->colorDB().size(); i++ )
		if ( VuGameUtil::IF()->colorDB()[i]["Name"].asString() == colorName )
			return i;

	return 0;
}

//*****************************************************************************
void VuColorWheelEntity::setIndex(int index)
{
	const std::string &colorName = VuGameUtil::IF()->colorDB()[index]["Name"].asString();

	const std::string &decal = VuGameUtil::IF()->getCarPaintDecal();
	const std::string &decalColor = VuGameUtil::IF()->getCarPaintDecalColor();
	const std::string &paintColor = VuGameUtil::IF()->getCarPaintPaintColor();

	if ( mType == PAINT_COLOR ) VuGameUtil::IF()->setCarPaint(decal, decalColor, colorName);
	if ( mType == DECAL_COLOR ) VuGameUtil::IF()->setCarPaint(decal, colorName, paintColor);
}


//*****************************************************************************
// VuDecalWheelEntity
//*****************************************************************************

class VuDecalWheelEntity : public VuWheelEntity
{
	DECLARE_RTTI

public:
	VuDecalWheelEntity();
	~VuDecalWheelEntity();

protected:
	virtual int			getItemCount();
	virtual VuColor		getItemColor(int index);
	virtual VuTexture	*getItemTexture(int index);
	virtual int			getIndex();
	virtual void		setIndex(int index);

	typedef std::vector<VuTextureAsset *> TextureAssets;
	TextureAssets	mTextureAssets;
};

IMPLEMENT_RTTI(VuDecalWheelEntity, VuWheelEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuDecalWheelEntity);

//*****************************************************************************
VuDecalWheelEntity::VuDecalWheelEntity()
{
	for ( int i = 0; i < VuGameUtil::IF()->decalDB().size(); i++ )
	{
		const std::string &textureName = VuGameUtil::IF()->decalDB()[i]["Texture"].asString();
		mTextureAssets.push_back(VuAssetFactory::IF()->createAsset<VuTextureAsset>(textureName, VuAssetFactory::OPTIONAL_ASSET));
	}
}

//*****************************************************************************
VuDecalWheelEntity::~VuDecalWheelEntity()
{
	for ( auto pTextureAsset : mTextureAssets )
		VuAssetFactory::IF()->releaseAsset(pTextureAsset);
}

//*****************************************************************************
int VuDecalWheelEntity::getItemCount()
{
	return VuGameUtil::IF()->decalDB().size();
}

//*****************************************************************************
VuColor VuDecalWheelEntity::getItemColor(int index)
{
	return VuColor(255,255,255);
}

//*****************************************************************************
VuTexture *VuDecalWheelEntity::getItemTexture(int index)
{
	VuTexture *pTexture = mTextureAssets[index]->getTexture();

	if ( pTexture == VUNULL )
		pTexture = VuGfxUtil::IF()->whiteTexture();

	return pTexture;
}

//*****************************************************************************
int VuDecalWheelEntity::getIndex()
{
	const char *decalName = "";

	decalName = VuGameUtil::IF()->getCarPaintDecal().c_str();

	for ( int i = 0; i < VuGameUtil::IF()->decalDB().size(); i++ )
		if ( VuGameUtil::IF()->decalDB()[i]["Name"].asString() == decalName )
			return i;

	return 0;
}

//*****************************************************************************
void VuDecalWheelEntity::setIndex(int index)
{
	const std::string &decalName = VuGameUtil::IF()->decalDB()[index]["Name"].asString();

	const std::string &decalColor = VuGameUtil::IF()->getCarPaintDecalColor();
	const std::string &paintColor = VuGameUtil::IF()->getCarPaintPaintColor();

	VuGameUtil::IF()->setCarPaint(decalName, decalColor, paintColor);
}
