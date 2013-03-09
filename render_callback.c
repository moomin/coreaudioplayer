#include "render_callback.h"

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
