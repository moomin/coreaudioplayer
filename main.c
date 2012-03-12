#include <stdio.h>
#include <AudioUnit/AudioComponent.h>
#include <AudioUnit/AudioUnit.h>

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
  printf ("Search result: %d\n", status);

  startPlay(&au);
  stopPlay(&au);
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

  //set renderer's function address
  renderer = myRenderer;

  void *nodata;
  status = AudioUnitAddRenderNotify(*ci, renderer, nodata);

  if (status != noErr)
  {
    return 3;
  }

  return 0;
}

int startPlay(AudioUnit *c)
{
  AudioStreamBasicDescription sf;
  sf.mSampleRate = 44100.0;
  sf.mFormatID = kAudioFormatLinearPCM;
  sf.mFormatFlags = 0;
  sf.mBytesInAPacket = 0;
  sf.mFramesPerPacket = 0;
  sf.mBytesPerFrame = 0;
  sf.mChannelsPerFrame =
  sf.mBitsPerChannel = 0;

  AudioUnitSetProperty(*c, kAudioUnitProperty_StreamFormat,
                       kAudioUnitScope_Input,
                       0, &sf, sizeof(AudioStreamBasicDescription));

  AudioUnitInitialize(*c);
  AudioOutputUnitStart(*c);
  return 0;
}

int stopPlay(AudioUnit *c)
{
  AudioOutputUnitStop(*c);
  AudioUnitUninitialize(*c);
  return 0;
}

int closeUnit(AudioUnit *c)
{
  AudioComponentInstanceDispose(*c);
  return 0; 
}

