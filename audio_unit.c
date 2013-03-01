#include "audio_unit.h"
//#include "render_callback.h"

int GetAudioUnit(AudioComponentInstance *ci, void *rendererFunction, void *rendererData)
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

  AURenderCallbackStruct input;
  input.inputProc = rendererFunction;
  input.inputProcRefCon = rendererData;

  status = AudioUnitSetProperty(*ci, kAudioUnitProperty_SetRenderCallback,
                                kAudioUnitScope_Input, 0, &input, sizeof(input));


  if (status != noErr)
  {
    return 3;
  }

  return 0;
}

int startPlay(AudioUnit *c)
{
  OSStatus status;
  AudioStreamBasicDescription sf = {0};
  UInt32 channels = 2;
  UInt32 bits = 16;

  sf.mSampleRate = 44100.0;
  sf.mFormatID = kAudioFormatLinearPCM;
  sf.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger |
                    kLinearPCMFormatFlagIsPacked |
                    kAudioFormatFlagsNativeEndian
;

  sf.mBitsPerChannel = bits;
  sf.mChannelsPerFrame = channels;
  sf.mBytesPerFrame = (bits / 8) * channels;
  sf.mFramesPerPacket = 1;
  sf.mBytesPerPacket = sf.mBytesPerFrame * sf.mFramesPerPacket;
  sf.mReserved = 0;

  status = AudioUnitSetProperty(*c,
                       kAudioUnitProperty_StreamFormat,
                       kAudioUnitScope_Input,
                       0, &sf, sizeof(AudioStreamBasicDescription));

  if (status != noErr)
  {
    return 1;
  }

  status = AudioUnitInitialize(*c);

  if (status != noErr)
  {
    return 2;
  }

  status = AudioOutputUnitStart(*c);
  
  if (status != noErr)
  {
    return 3;
  }

  SInt32 exit_reason;
  do {
  exit_reason = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.51, false);
  } while (1);

  if (status != noErr)
  {
    return 4;
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

