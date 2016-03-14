//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  UI Table class
// 
//*****************************************************************************

#pragma once

#include "VuEngine/UI/VuUIPropertyUtil.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Properties/VuAssetProperty.h"
#include "VuEngine/Properties/VuFontEnumProperty.h"

class VuScriptComponent;
class VuTransitionComponent;
class VuTextureAsset;
struct VuUIDrawParams;


class VuTableEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuTableEntity();
	~VuTableEntity();

	virtual void		onGameInitialize();

protected:
	// construction
	#define ADD_COLUMN(colName, enabledDefault)													\
	{																							\
		Column *pColumn = new Column(colName, enabledDefault);									\
		addProperty(new VuBoolProperty(colName" Enabled", pColumn->mEnabled));					\
		addProperty(new VuRectProperty(colName" Rect", pColumn->mRect));						\
		addProperty(new VuFontEnumProperty(colName" Font", pColumn->mFont));					\
		ADD_UI_STRING_FORMAT_PROPERTIES(getProperties(), pColumn->mStringFormat, colName" ");	\
		mColumns.push_back(pColumn);															\
	}

	virtual int			getRowCount() = 0;
	virtual const char	*getItemText(int row, int col) = 0;
	virtual bool		getRowHighlight(int row) { return false; }

	// event handlers
	void				OnUITick(const VuParams &params);
	void				OnUIDraw(const VuParams &params);

	VuRetVal			Show(const VuParams &params) { mVisible = true; return VuRetVal(); }
	VuRetVal			Hide(const VuParams &params) { mVisible = false; return VuRetVal(); }

	virtual void		drawLayout(bool bSelected);
	virtual void		drawTable(float alpha);
	void				drawElements(const VuUIDrawParams &uiDrawParams, int iRow, bool highlight, float alpha);

	struct Column
	{
		Column(const char *columnName, bool enabledDefault) : mColumnName(columnName), mEnabled(enabledDefault) {}
		std::string					mColumnName;
		bool						mEnabled;
		VuUIRectProperties			mRect;
		std::string					mFont;
		VuUIStringFormatProperties	mStringFormat;
	};
	typedef std::vector<Column *> Columns;

	// components
	VuScriptComponent		*mpScriptComponent;
	VuTransitionComponent	*mpTransitionComponent;

	// properties
	bool					mInitiallyVisible;
	int						mMaxRows;
	float					mSpacing;
	VuColor					mHighlightColor;
	VuUIRectProperties		mRowRect;
	std::string				mRowTexture;
	std::string				mRowHighlightTexture;
	VuUIRectProperties		mRowSrcRect;
	VuUIRectProperties		mRowHighlightSrcRect;
	Columns					mColumns;
	VuUIAnchorProperties	mAnchor;

	VuAssetProperty<VuTextureAsset>	*mpRowTextureAssetProperty;
	VuAssetProperty<VuTextureAsset>	*mpRowHighlightTextureAssetProperty;

	bool				mVisible;
};
