#include <AudioUnit/AudioUnit.h>

OSStatus sinRenderer(void *inRefCon,
                  AudioUnitRenderActionFlags *ioActionFlags,
                  const AudioTimeStamp *inTimeStamp,
                  UInt32 inBusNumber,
                  UInt32 inNumberFrames,
                  AudioBufferList *ioData);

OSStatus pcmRenderer(void *inRefCon,
                  AudioUnitRenderActionFlags *ioActionFlags,
                  const AudioTimeStamp *inTimeStamp,
                  UInt32 inBusNumber,
                  UInt32 inNumberFrames,
                  AudioBufferList *ioData);

typedef struct {
  //chunk
  char     chunkId[4];
  uint32_t chunkSize;
  char     format[4];

  //subchunk1
  char     subchunk1Id[4];
  uint32_t subchunk1Size;
  uint16_t audioFormat;
  uint16_t numChannels;
  uint32_t sampleRate;
  uint32_t byteRate;
  uint16_t blockAlign;
  uint16_t bitsPerSample;

  //subchunk2
  char     subchunk2Id[4];
  uint32_t subchunk2Size;
} wavHeader;

typedef struct {
  size_t bufferCapacity;

  void *startPtr;
  void *boundaryPtr;
  void *currentPtr;

  size_t bytesLeftA;
  size_t bytesLeftB;
  wavHeader header;
} pcmBuffer;
