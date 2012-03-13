#include <stdio.h>
#include <AudioUnit/AudioComponent.h>
#include <AudioUnit/AudioUnit.h>
#include <CoreFoundation/CoreFoundation.h>

int GetAudioUnit(AudioComponentInstance *ci);
int startPlay(AudioUnit *ci);
int stopPlay(AudioUnit *ci);
int closeUnit(AudioUnit *ci);

int main(int argc, char **argv)
{
  int status;
  AudioUnit au;

  printf("Hello World!\n");

  status = GetAudioUnit(&au);
  printf("Search result: %d\n", status);

  status = startPlay(&au);
  printf("startPlay: %d\n", status);

  status = stopPlay(&au);
  printf("stopPlay: %d\n", status);

  closeUnit(&au);

  return status;
}

OSStatus myRenderer(void *inRefCon,
                  AudioUnitRenderActionFlags *ioActionFlags,
                  const AudioTimeStamp inTimeStamp,
                  UInt32 inBusNumber,
                  UInt32 inNumberFrames,
                  AudioBufferList *ioData)
{
  printf("AURender: \n");
  return noErr;
}

int GetAudioUnit(AudioComponentInstance *ci)
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
  input.inputProc = myRenderer;
  input.inputProcRefCon = NULL;

  renderer = myRenderer;
  void *vd;

  status = AudioUnitAddRenderNotify(*ci, renderer, vd);

/*
  status = AudioUnitSetProperty(*ci, kAudioUnitProperty_SetRenderCallback,
                                kAudioUnitScope_Input, 0, &input, sizeof(input));
*/

  if (status != noErr)
  {
    return 3;
  }

  return 0;
}

int startPlay(AudioUnit *c)
{
  OSStatus status;
  AudioStreamBasicDescription sf;
  sf.mSampleRate = 44100.0;
  sf.mFormatID = kAudioFormatLinearPCM;
  sf.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger |
                    kLinearPCMFormatFlagIsPacked |
                    kAudioFormatFlagsNativeEndian |
                    kAudioFormatFlagIsNonInterleaved;
  sf.mBytesPerPacket = 2;
  sf.mFramesPerPacket = 1;
  sf.mBytesPerFrame = 2;
  sf.mChannelsPerFrame = 2;
  sf.mBitsPerChannel = 16;

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

  CFRunLoopRunInMode(kCFRunLoopDefaultMode, 1, false);

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

