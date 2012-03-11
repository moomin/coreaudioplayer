#include <stdio.h>
#include <AudioUnit/AudioComponent.h>
#include <AudioUnit/AudioUnit.h>

int GetAudioUnit();

int main(int argc, char **argv)
{
  printf("Hello World!\n");

  printf ("Search result: %d\n", GetAudioUnit());
  return 0;
}

OSStatus AURender(void *inRefCon,
                  AudioUnitRenderActionFlags *ioActionFlags,
                  const AudioTimeStamp inTimeStamp,
                  UInt32 inBusNumber,
                  UInt32 inNumberFrames,
                  AudioBufferList *ioData)
{
  printf("AURender: \n");
  return noErr;
}

int GetAudioUnit()
{
  AudioComponent component;
  AudioComponentDescription dsc;
  AudioComponentInstance ci;
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
  status = AudioComponentInstanceNew(component, &ci);

  if (status != noErr)
  {
    return 2;
  }

  //initialize the AU
  status = AudioUnitInitialize(ci);

  if (status != noErr)
  {
    return 3;
  }

  //set renderer's function address
  renderer = &AURender;

  void *nodata;
  status = AudioUnitAddRenderNotify(ci, renderer, nodata);

  if (status != noErr)
  {
    return 4;
  }

  status = AudioUnitRender(ci, 0, );

  return 0;
}
