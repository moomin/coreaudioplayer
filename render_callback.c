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

OSStatus pcmRenderer(void *inRefCon,
                  AudioUnitRenderActionFlags *ioActionFlags,
                  const AudioTimeStamp *inTimeStamp,
                  UInt32 inBusNumber,
                  UInt32 inNumberFrames,
                  AudioBufferList *ioData)
{
  pcmBuffer *pcm;
  pcm = (pcmBuffer *)inRefCon;
  int *outputBuffer = (int *)ioData->mBuffers[0].mData;

  //reset wav->currentPtr to wav->startPtr if currentPtr is out of bounds
  if (pcm->currentPtr == (pcm->startPtr + pcm->bufferCapacity))
  {
    pcm->currentPtr = pcm->startPtr;
  }

  for (int i = 0; i < inNumberFrames; i++)
  {
    //render silence if there is nothing to play
    if ((pcm->bytesLeftA <= 0) && (pcm->bytesLeftB <= 0))
    {
      outputBuffer[i] = (int)0;
      continue;
    }

    outputBuffer[i] = *((int *)pcm->currentPtr);

    if (pcm->currentPtr < pcm->boundaryPtr)
    {
      pcm->bytesLeftA -= sizeof(int);
    }
    else
    {
      pcm->bytesLeftB -= sizeof(int);
    }

    //advance the position in the buffer
    pcm->currentPtr += sizeof(int);
  }

  return noErr;
}
