#include <stdio.h>
#include "render_callback.h"
#include "audio_unit.h"
#include "read_file.h"

#define RENDER_FILE true
#define BUFFER_CAPACITY_KB 1024

int playSineWave();
int playWavFile(char *filename);
int prepareBuffer(char *filename, char *buffer, size_t bufferSize);

int main(int argc, char **argv)
{
  int status;

  if (RENDER_FILE)
  {
    return playWavFile("./sample.wav");
  }
  else
  {
    return playSineWave();
  }
}

int playSineWave()
{
  int status;
  double renderPhase;
  AudioUnit au;

  status = setupAudioUnit(&au, sinRenderer, &renderPhase);

  if (status != noErr)
  {
    closeUnit(&au);
    printf("Error setting up an AudioUnit. Exiting\n");
    return 1;
  }

  printf("starting to play sine wave\n");
  status = startPlay(&au);

  //let it play for 10 seconds
  usleep(10000000);

  printf("stopping playback\n");
  status = stopPlay(&au);

  closeUnit(&au);

  return status;
}

int playWavFile(char *filename)
{
  int status;
  AudioUnit au;

  if (getAudioUnit(&au) != noErr)
  {
    printf("Cannot obtain AudioUnit component. Exiting\n");
    return 1;
  }

  printf("File has been successfully read into the buffer\n");

  //setup audioBuffer info for renderer
  wavBuffer wav;
  wav.bufferCapacity = BUFFER_CAPACITY_KB * 1024;
  wav.startPtr = calloc(BUFFER_CAPACITY_KB, 1024);
  wav.boundaryPtr = wav.startPtr + (BUFFER_CAPACITY_KB * 1024 / 2);
  wav.currentPtr = wav.startPtr;
  wav.bytesLeftA = 0;
  wav.bytesLeftB = 0;

  status = setupAudioUnit(&au, wavRenderer, &wav);

  if (status != noErr)
  {
    closeUnit(&au);
    printf("Error setting up an AudioUnit. Exiting\n");
    return 1;
  }

  printf("starting playback\n");
  status = startPlay(&au);

  feedTheBuffer("./sample.wav", &wav);

  printf("stopping playback\n");
  status = stopPlay(&au);

  closeUnit(&au);

  return status;
}

