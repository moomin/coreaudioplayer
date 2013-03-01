#include <stdio.h>
#include "render_callback.h"

OSStatus myRenderer(void *inRefCon,
                  AudioUnitRenderActionFlags *ioActionFlags,
                  const AudioTimeStamp *inTimeStamp,
                  UInt32 inBusNumber,
                  UInt32 inNumberFrames,
                  AudioBufferList *ioData)
{
  double currPhase = *((double *)inRefCon);
  Float32 *outputBuffer = (Float32 *)ioData->mBuffers[0].mData;
  const double frequency = 2.;
  const double phaseStep = (frequency / 44100.) * (M_PI * 2.);
  
  for (int i = 0; i < inNumberFrames; i++)
  {
    outputBuffer[i] = sin(currPhase);
    currPhase += phaseStep;
  }

  *((double *)inRefCon) = currPhase;

  printf("ioData address: %p; inNumberFrames: %d; flags: %d\n", ioData, inNumberFrames, *ioActionFlags);
  return noErr;
}

