#include <unistd.h>
#include "render_callback.h"
#include "audio_unit.h"

int getAudioUnit(AudioComponentInstance *ci)
{
  AudioComponent component;
  AudioComponentDescription dsc;
  OSStatus status;
  AURenderCallback renderer;

  //specify component search criteria
  dsc.componentType = kAudioUnitType_Output;
  dsc.componentSubType = kAudioUnitSubType_DefaultOutput;
  dsc.componentManufacturer = kAudioUnitManufacturer_Apple;
  dsc.componentFlags = 0;
  dsc.componentFlagsMask = 0;

  //find next component
  component = AudioComponentFindNext(NULL, &dsc);

  if (component == NULL)
  {
    return 1;
  }

  //get AudioUnit instance
  status = AudioComponentInstanceNew(component, ci);

  if (status != noErr)
  {
    return 2;
  }

  return 0;
}

int setupAudioUnit(AudioUnit *au, void *rendererFunction, void *rendererData)
{
  OSStatus status;
  wavHeader wavhdr;
  AURenderCallbackStruct input;

  input.inputProc = rendererFunction;
  input.inputProcRefCon = rendererData;

  status = AudioUnitSetProperty(*au, kAudioUnitProperty_SetRenderCallback,
                                kAudioUnitScope_Input, 0, &input, sizeof(input));

  if (status != noErr)
  {
    return 1;
  }

  wavhdr = (wavHeader)((pcmBuffer *)rendererData)->header;

  //fill-in stream description
  AudioStreamBasicDescription sf = {0};

  sf.mSampleRate = (Float64)wavhdr.sampleRate;
  sf.mFormatID = kAudioFormatLinearPCM;
  sf.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger |
                    kLinearPCMFormatFlagIsPacked |
                    kAudioFormatFlagsNativeEndian
;

  sf.mBitsPerChannel = wavhdr.bitsPerSample;
  sf.mChannelsPerFrame = wavhdr.numChannels;
  sf.mBytesPerFrame = (wavhdr.bitsPerSample / 8) * wavhdr.numChannels;
  sf.mFramesPerPacket = 1;
  sf.mBytesPerPacket = sf.mBytesPerFrame * sf.mFramesPerPacket;
  sf.mReserved = 0;

  status = AudioUnitSetProperty(*au,
                       kAudioUnitProperty_StreamFormat,
                       kAudioUnitScope_Input,
                       0, &sf, sizeof(AudioStreamBasicDescription));

  if (status != noErr)
  {
    return 2;
  }

  return 0;
}

int startPlay(AudioUnit *c)
{
  OSStatus status;

  status = AudioUnitInitialize(*c);

  if (status != noErr)
  {
    return 1;
  }

  status = AudioOutputUnitStart(*c);
  
  if (status != noErr)
  {
    return 2;
  }

  return 0;
}

int stopPlay(AudioUnit *c)
{
  OSStatus status;

  status = AudioOutputUnitStop(*c);

  if (status != noErr)
  {
    return 1;
  }

  status = AudioUnitUninitialize(*c);
  if (status != noErr)
  {
    return 2;
  }

  return 0;
}

int closeUnit(AudioUnit *c)
{
  AudioComponentInstanceDispose(*c);
  return 0; 
}

