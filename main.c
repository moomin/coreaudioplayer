#include <stdio.h>
#include "render_callback.h"
#include "audio_unit.h"
#include "read_file.h"

#define RENDER_FILE true
#define BUFFER_CAPACITY_KB 1024

int playSineWave();
int playWavFile(char *filename);

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Usage: cap <filename>\n");
    exit(1);
  }

  

  int status;

  if (RENDER_FILE)
  {
    return playWavFile(argv[1]);
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

  feedTheBuffer(filename, &wav);

  printf("stopping playback\n");
  status = stopPlay(&au);

  closeUnit(&au);

  return status;
}

