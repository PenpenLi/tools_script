//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Ghost Util
// 
//*****************************************************************************

#pragma once

#include "VuEngine/Containers/VuArray.h"
#include "VuEngine/Math/VuVector3.h"
#include "VuEngine/Util/VuBinaryDataUtil.h"

class VuCarEntity;


class VuGhostRecorder
{
public:
	VuGhostRecorder(VuCarEntity *pCar);

	void		startRecord(float maxTime, float timeStep);
	void		endRecord();

	void		tickRecord(float fdt);
	void		recordFrame();

	VuCarEntity			*mpCar;
	VuArray<VUBYTE>		mData;
	VuBinaryDataWriter	mWriter;

	float				mMaxTime;
	float				mTimeStep;
	float				mTime;
	float				mTimeAccum;
};

// record buffer entries
namespace VuGhostUtil
{
	enum { INITIAL_STATE, FRAME_V1_DEPRECATED, FINAL_STATE, FRAME_V2, FRAME_V3 };

	enum
	{
		FLAG_GHOST = (1<<0),
		FLAG_WHEEL0_SLIDING = (1<<1),
		FLAG_WHEEL1_SLIDING = (1<<2),
		FLAG_WHEEL2_SLIDING = (1<<3),
		FLAG_WHEEL3_SLIDING = (1<<4),
		FLAG_BACKFIRE_PFX = (1<<5),
	};

	VUUINT64	encodePosition64(float x, float y, float z);
	void		decodePosition64(VUUINT64 pos, float &x, float &y, float &z);
	VUUINT32	encodeRotation32(float x, float y, float z);
	void		decodeRotation32(VUUINT32 rot, float &x, float &y, float &z);

	struct InitialState
	{
		void		write(VuBinaryDataWriter &writer);
		void		read(VuBinaryDataReader &reader);

		std::string	mCarName;
		int			mStage;
		std::string	mDriverName;
		std::string	mDecalName;
		std::string	mPaintColorName;
		std::string	mDecalColorName;

		VuVector3	mPosition;
		VuVector3	mRotation;
	};

	struct Frame
	{
		void		write(VuBinaryDataWriter &writer);
		void		readV3(VuBinaryDataReader &reader);
		void		readV2(VuBinaryDataReader &reader);

		float		mTime;
		VuVector3	mPosition;
		VuVector3	mRotation;
		float		mSteering;
		float		mAudioRPM;
		float		mAudioLoad;
		VUUINT8		mFlags;
	};
}