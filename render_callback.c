#include <stdio.h>
#include "render_callback.h"

OSStatus sinRenderer(void *inRefCon,
                  AudioUnitRenderActionFlags *ioActionFlags,
                  const AudioTimeStamp *inTimeStamp,
                  UInt32 inBusNumber,
                  UInt32 inNumberFrames,
                  AudioBufferList *ioData)
{
  double currPhase = *((double *)inRefCon);
  int *outputBuffer = (int *)ioData->mBuffers[0].mData;
  const double frequency = 440.;
  const double phaseStep = (frequency / 44100.) * (M_PI * 2.);
  
  for (int i = 0; i < inNumberFrames; i++)
  {
    outputBuffer[i] = (int)(sin(currPhase)*32768) << 16 | (int)(sin(currPhase)*32768);
    currPhase += phaseStep;
  }

  *((double *)inRefCon) = currPhase;

  printf("currPhase: %f; inNumberFrames: %d; outputBuffer(%lu): %d\n", currPhase, inNumberFrames, sizeof(outputBuffer[0]), *((int *)outputBuffer));
  return noErr;
}

OSStatus fileRenderer(void *inRefCon,
                  AudioUnitRenderActionFlags *ioActionFlags,
                  const AudioTimeStamp *inTimeStamp,
                  UInt32 inBusNumber,
                  UInt32 inNumberFrames,
                  AudioBufferList *ioData)
{
  int32_t currFrame = *((int32_t *)inRefCon);
  int *outputBuffer = (int *)ioData->mBuffers[0].mData;

	for (int i = 0; i < inNumberFrames; i++)
	{
    outputBuffer[i] = currFrame;
		inRefCon += sizeof(int32_t);
		currFrame = *((int32_t *)inRefCon);
	}

  printf("inNumberFrames: %d; currFrame = %d\n", inNumberFrames, currFrame);
  return noErr;
}
