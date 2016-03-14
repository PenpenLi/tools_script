//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  VuIosVideoShareManager
//
//*****************************************************************************

#import <Kamcord/Kamcord.h>
#import <CoreMedia/CMFormatDescription.h>


#include "VuEngine/HAL/Gfx/Ios/VuMetalGfx.h"
#include "VuEngine/HAL/Audio/VuAudio.h"
#include "VuPurple/Managers/VuVideoShareManager.h"


class VuIosVideoShareManager : public VuVideoShareManager
{
public:
	VuIosVideoShareManager();
	
protected:
	virtual bool	init();
	virtual void	release();
	
    virtual bool	isSupported_internal();
	
	virtual void	startRecording_internal();
	virtual void	stopRecording_internal();
	virtual void	pause_internal()			{ [Kamcord pause]; }
	virtual void	resume_internal()			{ [Kamcord resume]; }
	virtual void	beginDraw_internal()		{}
	virtual void	endDraw_internal()			{}
	virtual void	showShareUI_internal()		{ [Kamcord showView]; }
	virtual void	showWatchUI_internal()		{ [Kamcord showWatchView]; }
	
	static FMOD_RESULT F_CALLBACK fmodCallback(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int outchannels);
	
	CMFormatDescriptionRef	mAudioFormatDescription;
	FMOD::DSP				*mpDsp;
};

// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuVideoShareManager, VuIosVideoShareManager);


//*****************************************************************************
VuIosVideoShareManager::VuIosVideoShareManager():
	mpDsp(VUNULL)
{
}

//*****************************************************************************
bool VuIosVideoShareManager::init()
{
	if ( !VuVideoShareManager::init() )
		return false;
	
	if ( isSupported_internal() )
	{
		// configure FMOD dsp to capture audio
		FMOD_DSP_DESCRIPTION dspdesc;
		memset(&dspdesc, 0, sizeof(FMOD_DSP_DESCRIPTION));
		
		strcpy(dspdesc.name, "Kamcord Read Callback Custom DSP");
		dspdesc.channels = 0;
		dspdesc.read = fmodCallback;
		dspdesc.userdata = (void *)0x12345678;
		
		int fmodSampleRate = 0;
		int fmodNumOutputChannels;
		int fmodMaxInputChannels;
		
		FMOD_SOUND_FORMAT fmodSoundFormat;
		FMOD_DSP_RESAMPLER fmodResampleMethod;
		
		int fmodBits;
		VuAudio::IF()->system()->getSoftwareFormat(&fmodSampleRate, &fmodSoundFormat, &fmodNumOutputChannels, &fmodMaxInputChannels, &fmodResampleMethod, &fmodBits);
		
		AudioStreamBasicDescription asbd;
		bzero(&asbd, sizeof(asbd));
		asbd.mSampleRate = fmodSampleRate;
		
		// We only handle PCM for now.
		asbd.mFramesPerPacket = 1;
		asbd.mChannelsPerFrame = fmodNumOutputChannels;
		int bytes_per_sample = sizeof(float);
		
		// Default to 8 bit PCM.
		asbd.mFormatID = kAudioFormatLinearPCM;
		asbd.mBitsPerChannel = bytes_per_sample * 8;
		
		asbd.mFormatFlags = kAudioFormatFlagIsFloat | kAudioFormatFlagIsAlignedHigh;
		asbd.mBytesPerFrame = bytes_per_sample * asbd.mChannelsPerFrame;
		asbd.mBytesPerPacket = asbd.mBytesPerFrame * asbd.mFramesPerPacket;
		
		CMAudioFormatDescriptionCreate(kCFAllocatorDefault, &asbd, 0, NULL, 0, NULL, NULL, &mAudioFormatDescription);
		
		// Tells Kamcord the audio format
		[Kamcord setASBD:asbd];
		
		// create/activate/add DSP
		FMODCALL(VuAudio::IF()->system()->createDSP(&dspdesc, &mpDsp));
		FMODCALL(mpDsp->setBypass(false));
	}
	
	return true;
}

//*****************************************************************************
void VuIosVideoShareManager::release()
{
	if ( mpDsp )
	{
		mpDsp->release();
		mpDsp = VUNULL;
	}
	
	VuVideoShareManager::release();
}

//*****************************************************************************
bool VuIosVideoShareManager::isSupported_internal()
{
    // Right now, we assume that if Metal is present, there's no video capture
    // capability, so Kamcord is NOT supported until they fix their stuff.
    //
    if (VuMetalGfx::checkForMetal() == true)
    {
        return false;
    }
    
    // Otherwise, return the result of Kamcord's device whitelist.
    return [Kamcord isEnabled];
}

//*****************************************************************************
void VuIosVideoShareManager::startRecording_internal()
{
	FMODCALL(VuAudio::IF()->system()->addDSP(mpDsp, VUNULL));
	
	[Kamcord startRecording];
}

//*****************************************************************************
void VuIosVideoShareManager::stopRecording_internal()
{
	[Kamcord stopRecording];

	mpDsp->remove();
}

//*****************************************************************************
FMOD_RESULT F_CALLBACK VuIosVideoShareManager::fmodCallback(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int outchannels)
{
	// This loop assumes inchannels = outchannels, which it will be if the DSP is created with '0'
	// as the number of channels in FMOD_DSP_DESCRIPTION.
	// Specifying an actual channel count will mean you have to take care of any number of channels coming in,
	// but outputting the number of channels specified. Generally it is best to keep the channel
	// count at 0 for maximum compatibility.
	
	for (int chan = 0; chan < outchannels; chan++)
	{
		for (unsigned int sample = 0; sample < length; sample++)
		{
			outbuffer[(sample * outchannels) + chan] = inbuffer[(sample * inchannels) + chan];
		}
	}
	
	// Feeds the audio bytes to Kamcord
	[Kamcord writeAudioBytes:inbuffer numSamples:length];
	
	return FMOD_OK;
}
