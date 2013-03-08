#include <AudioUnit/AudioUnit.h>

OSStatus sinRenderer(void *inRefCon,
                  AudioUnitRenderActionFlags *ioActionFlags,
                  const AudioTimeStamp *inTimeStamp,
                  UInt32 inBusNumber,
                  UInt32 inNumberFrames,
                  AudioBufferList *ioData);

OSStatus wavRenderer(void *inRefCon,
                  AudioUnitRenderActionFlags *ioActionFlags,
                  const AudioTimeStamp *inTimeStamp,
                  UInt32 inBusNumber,
                  UInt32 inNumberFrames,
                  AudioBufferList *ioData);



typedef struct {
  int chunkSize;
  
} streamFormat;

typedef struct {
  size_t bufferCapacity;

  void *startPtr;
  void *boundaryPtr;
  void *currentPtr;

  size_t bytesLeftA;
  size_t bytesLeftB;
} wavBuffer;
