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
  audioBuffer *ab;
  ab = (audioBuffer *)inRefCon;
  int *outputBuffer = (int *)ioData->mBuffers[0].mData;
  short enough = 0;

  for (int i = 0; i < inNumberFrames; i++)
  {
    //render silence if the buffer ended
    if (ab->playedBytes >= ab->totalBytes)
    {
      outputBuffer[i] = (int)0;
      continue;
    }

    outputBuffer[i] = *((int *)ab->audioDataPos);

    //move position in the buffer
    ab->audioDataPos += sizeof(int);
    //remember how many bytes we already retrieved from the buffer
    ab->playedBytes += sizeof(int);

  }

  return noErr;
}
