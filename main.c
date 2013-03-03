#include <stdio.h>
#include "render_callback.h"
#include "audio_unit.h"
#include "read_file.h"

#define RENDER_FILE true

int prepareBuffer(char *filename, char *buffer, size_t bufferSize);

int main(int argc, char **argv)
{
  int status;
  AudioUnit au;

  double renderPhase;

  if (getAudioUnit(&au) != noErr)
  {
    printf("Cannot obtain AudioUnit component. Exiting\n");
    return 1;
  }

  if (RENDER_FILE)
  {
    void *buffer;
    size_t bufferSize = 1024 * 1024;
    buffer = calloc(1024, 1024);

    if (prepareBuffer("./sample.wav", buffer, bufferSize) != 0)
    {
      printf("Error reading file into the buffer. Exiting\n");
      return 1;
    }

    printf("File has been successfully read into the buffer\n");

    //setup audioBuffer info for renderer
    audioBuffer ab;
    ab.audioDataPos = buffer;
    ab.playedBytes = 0;
    ab.totalBytes = bufferSize;
    ab.au = &au;

    status = setupAudioUnit(&au, fileRenderer, &ab);
  }
  else
  {
    status = setupAudioUnit(&au, sinRenderer, &renderPhase);
  }

  if (status != noErr)
  {
    closeUnit(&au);
    printf("Error setting up an AudioUnit. Exiting\n");
    return 1;
  }

  printf("starting playback\n");
  status = startPlay(&au);

  printf("stopping playback\n");
  status = stopPlay(&au);

  closeUnit(&au);

  return status;
}

int prepareBuffer(char *filename, char *buffer, size_t bufferSize)
{
  return readFile(filename, buffer, bufferSize);
}
