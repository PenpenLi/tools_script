//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Ghost Util
// 
//*****************************************************************************

#include "VuGhostUtil.h"
#include "VuPurple/Entities/Car/VuCarEntity.h"
#include "VuPurple/Entities/Car/Parts/VuCarEngine.h"
#include "VuPurple/Entities/Car/Parts/VuCarSuspension.h"
#include "VuPurple/Entities/Car/Parts/VuCarPfxController.h"
#include "VuEngine/Managers/VuTickManager.h"


//*****************************************************************************
VuGhostRecorder::VuGhostRecorder(VuCarEntity *pCar):
	mpCar(pCar),
	mWriter(mData),
	mMaxTime(FLT_MAX),
	mTimeStep(0.0f),
	mTime(0.0f),
	mTimeAccum(0.0f)
{
}

//*****************************************************************************
void VuGhostRecorder::startRecord(float maxTime, float timeStep)
{
	// record initial state
	VuGhostUtil::InitialState initialState;

	initialState.mCarName = mpCar->getCarName();
	initialState.mStage = mpCar->getCarStage();
	initialState.mDriverName = mpCar->getDriverName();
	initialState.mDecalName = mpCar->getDecalName();
	initialState.mPaintColorName = mpCar->getPaintColorName();
	initialState.mDecalColorName = mpCar->getDecalColorName();

	initialState.mPosition = mpCar->getModelPosition();
	initialState.mRotation = mpCar->getModelRotation();

	initialState.write(mWriter);

	mMaxTime = maxTime;
	mTimeStep = timeStep;
	mTime = 0.0f;
	mTimeAccum = 0.0f;

	recordFrame();

	VuTickManager::IF()->registerHandler(this, &VuGhostRecorder::tickRecord, "Final");
}

//*****************************************************************************
void VuGhostRecorder::endRecord()
{
	VuTickManager::IF()->unregisterHandlers(this);
}

//*****************************************************************************
void VuGhostRecorder::tickRecord(float fdt)
{
	if ( mTime < mMaxTime )
	{
		mTime += fdt;
		mTimeAccum += fdt;
		if ( mTimeAccum >= mTimeStep )
		{
			recordFrame();

			if ( mTimeStep )
				mTimeAccum -= mTimeStep*VuFloor(mTimeAccum/mTimeStep);
			else
				mTimeAccum = 0.0f;
		}
	}
}

//*****************************************************************************
void VuGhostRecorder::recordFrame()
{
	VuGhostUtil::Frame frame;

	frame.mTime = mTime;
	frame.mPosition = mpCar->getModelPosition();
	frame.mRotation = mpCar->getModelRotation();
	frame.mSteering = mpCar->getYawControl();
	frame.mAudioRPM = VuLinStep(mpCar->getEngine()->getMinRPM(), mpCar->getEngine()->getMaxRPM(), mpCar->getEngine()->getAudioRPM());
	frame.mAudioLoad = mpCar->getEngine()->getAudioLoad();

	frame.mFlags = 0;
	if ( mpCar->isInGhostMode() )
		frame.mFlags |= VuGhostUtil::FLAG_GHOST;

	if ( mpCar->getWheel(0).sliding() ) frame.mFlags |= VuGhostUtil::FLAG_WHEEL0_SLIDING;
	if ( mpCar->getWheel(1).sliding() ) frame.mFlags |= VuGhostUtil::FLAG_WHEEL1_SLIDING;
	if ( mpCar->getWheel(2).sliding() ) frame.mFlags |= VuGhostUtil::FLAG_WHEEL2_SLIDING;
	if ( mpCar->getWheel(3).sliding() ) frame.mFlags |= VuGhostUtil::FLAG_WHEEL3_SLIDING;

	if ( mpCar->getPfxController()->isBackFirePfxOn() )
		frame.mFlags |= VuGhostUtil::FLAG_BACKFIRE_PFX;

	frame.write(mWriter);
}

//*****************************************************************************
void VuGhostUtil::InitialState::write(VuBinaryDataWriter &writer)
{
	VUUINT8 type = INITIAL_STATE;
	writer.writeValue(type);

	writer.writeString(mCarName);
	writer.writeValue(mStage);
	writer.writeString(mDriverName);
	writer.writeString(mDecalName);
	writer.writeString(mPaintColorName);
	writer.writeString(mDecalColorName);

	writer.writeValue(mPosition.mX);
	writer.writeValue(mPosition.mY);
	writer.writeValue(mPosition.mZ);

	writer.writeValue(mRotation.mX);
	writer.writeValue(mRotation.mY);
	writer.writeValue(mRotation.mZ);
}

//*****************************************************************************
void VuGhostUtil::InitialState::read(VuBinaryDataReader &reader)
{
	VUUINT8 type;
	reader.readValue(type);
	VUASSERT(type == INITIAL_STATE, "VuGhostUtil::InitialState read error");

	reader.readString(mCarName);
	reader.readValue(mStage);
	reader.readString(mDriverName);
	reader.readString(mDecalName);
	reader.readString(mPaintColorName);
	reader.readString(mDecalColorName);

	reader.readValue(mPosition.mX);
	reader.readValue(mPosition.mY);
	reader.readValue(mPosition.mZ);

	reader.readValue(mRotation.mX);
	reader.readValue(mRotation.mY);
	reader.readValue(mRotation.mZ);
}

//*****************************************************************************
void VuGhostUtil::Frame::write(VuBinaryDataWriter &writer)
{
	VUUINT8 type = FRAME_V3;
	writer.writeValue(type);

	VUUINT64 encPos = encodePosition64(mPosition.mX, mPosition.mY, mPosition.mZ);
	VUUINT32 encRot = encodeRotation32(mRotation.mX, mRotation.mY, mRotation.mZ);
	VUINT8 encSteering = (VUINT8)VuRound(127.0f*mSteering);
	VUUINT8 encAudioRPM = (VUUINT8)VuRound(255.0f*mAudioRPM);
	VUUINT8 encAudioLoad = (VUUINT8)VuRound(255.0f*mAudioLoad);

	writer.writeValue(mTime);
	writer.writeValue(encPos);
	writer.writeValue(encRot);
	writer.writeValue(encSteering);
	writer.writeValue(encAudioRPM);
	writer.writeValue(encAudioLoad);
	writer.writeValue(mFlags);
}

//*****************************************************************************
void VuGhostUtil::Frame::readV3(VuBinaryDataReader &reader)
{
	VUUINT8 type;
	reader.readValue(type);
	VUASSERT(type == FRAME_V3, "VuGhostUtil::Frame read error");

	VUUINT64 encPos;
	VUUINT32 encRot;
	VUINT8 encSteering;
	VUUINT8 encAudioRPM;
	VUUINT8 encAudioLoad;

	reader.readValue(mTime);
	reader.readValue(encPos);
	reader.readValue(encRot);
	reader.readValue(encSteering);
	reader.readValue(encAudioRPM);
	reader.readValue(encAudioLoad);
	reader.readValue(mFlags);

	decodePosition64(encPos, mPosition.mX, mPosition.mY, mPosition.mZ);
	decodeRotation32(encRot, mRotation.mX, mRotation.mY, mRotation.mZ);
	mSteering = encSteering*(1.0f/127.0f);
	mAudioRPM = encAudioRPM*(1.0f/255.0f);
	mAudioLoad = encAudioLoad*(1.0f/255.0f);
}

//*****************************************************************************
void VuGhostUtil::Frame::readV2(VuBinaryDataReader &reader)
{
	VUUINT8 type;
	reader.readValue(type);
	VUASSERT(type == FRAME_V2, "VuGhostUtil::Frame read error");

	VUUINT64 encPos;
	VUUINT32 encRot;
	VUINT8 encSteering;

	reader.readValue(mTime);
	reader.readValue(encPos);
	reader.readValue(encRot);
	reader.readValue(encSteering);
	reader.readValue(mFlags);

	decodePosition64(encPos, mPosition.mX, mPosition.mY, mPosition.mZ);
	decodeRotation32(encRot, mRotation.mX, mRotation.mY, mRotation.mZ);
	mSteering = encSteering*(1.0f/127.0f);
	mAudioRPM = 1.0f;
	mAudioLoad = 1.0f;
}

//*****************************************************************************
VUUINT64 VuGhostUtil::encodePosition64(float x, float y, float z)
{
	// x
	// 1 bit for sign
	// 13 bits for whole number
	// 8 bits for fraction
	// 22 bits

	// y
	// 1 bit for sign
	// 13 bits for whole number
	// 8 bits for fraction
	// 22 bits

	// z
	// 1 bit for sign
	// 11 bits for whole number
	// 8 bits for fraction
	// 20 bits

	const float xyRange = 8191.0f;//powf(2, 13)-1
	const float zRange = 2047.0f;//powf(2, 11)-1

	if ( VuAbs(x) > xyRange )
		VUPRINT("X coordinate to be encoded exceeds bound");

	if ( VuAbs(y) > xyRange )
		VUPRINT("Y coordinate to be encoded exceeds bound");

	if ( VuAbs(z) > zRange )
		VUPRINT("Z coordinate to be encoded exceeds bound");

	const float xyScale = 256.0f;//powf(2, 8)
	const float zScale = 256.0f;//powf(2, 8)

	VUINT64 tx = (VUINT64)(x * xyScale);
	VUINT64 ty = (VUINT64)(y * xyScale);
	VUINT64 tz = (VUINT64)(z * zScale);

	VUUINT64 temp = (tx & 0x3FFFFF) << 42;
	temp |= (ty & 0x3FFFFF) << 20;
	temp |= (tz & 0xFFFFF);

	return temp;
}

//*****************************************************************************
void VuGhostUtil::decodePosition64(VUUINT64 pos, float &x, float &y, float &z)
{
	VUINT32 tx = (VUINT32)(pos >> 42);
	VUINT32 ty = (VUINT32)((pos >> 20) & 0x3FFFFF);
	VUINT32 tz = (VUINT32)(pos & 0xFFFFF);

	tx = (tx & 0x200000) ? (tx | 0xFFC00000) : tx;
	ty = (ty & 0x200000) ? (ty | 0xFFC00000) : ty;
	tz = (tz & 0x80000) ? (tz | 0xFFF80000) : tz;

	const float xyScale = 256.0f;
	const float zScale = 256.0f;

	x = (float)tx / xyScale;
	y = (float)ty / xyScale;	
	z = (float)tz / zScale;
}

//*****************************************************************************
VUUINT32 VuGhostUtil::encodeRotation32(float x, float y, float z)
{
	// 1 bit for sign
	// 3 bits for whole number
	// 6 bits for fraction
	const float xRange = 7.0f;//powf(2, 3)-1

	// 1 bit for sign
	// 3 bits for whole number
	// 7 bits for fraction
	const float yzRange = 7.0f;//powf(2, 3)-1

	if ( VuAbs(x) > xRange )
		VUPRINT("X component of angle to be encoded exceeds bound");

	if(VuAbs(y) > yzRange)
		VUPRINT("Y component of angle to be encoded exceeds bound");

	if(VuAbs(z) > yzRange)
		VUPRINT("Z component of angle to be encoded exceeds bound");

	const float xScale	= 64.0f;//powf(2, 6);
	const float yScale	= 128.0f;//powf(2, 7)
	const float zScale	= 128.0f;//powf(2, 7)

	VUINT32 tx		= (VUINT32)(x * xScale);
	VUINT32 ty		= (VUINT32)(y * yScale);
	VUINT32 tz		= (VUINT32)(z * zScale);

	VUUINT32 temp = (tx & 0x3FF) << 22;
	temp |= (ty & 0x7FF) << 11;
	temp |= (tz & 0x7FF);

	return temp;
}

//*****************************************************************************
void VuGhostUtil::decodeRotation32(VUUINT32 rot, float &x, float &y, float &z)
{
	VUINT32 tx = (VUINT32)((rot & 0xFFC00000) >> 22);
	VUINT32 ty = (VUINT32)((rot & 0x3FF800) >> 11);
	VUINT32 tz = (VUINT32)((rot & 0x7FF));

	tx = (tx & 0x200) ? (tx | 0xFFFFFC00) : tx;
	ty = (ty & 0x400) ? (ty | 0xFFFFF800) : ty;
	tz = (tz & 0x400) ? (tz | 0xFFFFF800) : tz;

	const float xScale = 64.0f;
	const float yScale = 128.0f;
	const float zScale = 128.0f;

	x = tx / xScale;
	y = ty / yScale;
	z = tz / zScale;
}
