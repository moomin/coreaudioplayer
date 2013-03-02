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

  if (RENDER_FILE)
  {
    void *buffer;
    size_t bufferSize = 1024 * 1024;
    buffer = calloc(1024, 1024);

    if (prepareBuffer("./sample.wav", buffer, bufferSize) != 0)
    {
      printf("error reading file into the buffer\n");
      return 1;
    }

    printf("file has been successfully read into the buffer\n");

    //setup audioBuffer info for renderer
    audioBuffer ab;
    ab.audioDataPos = buffer;
    ab.playedBytes = 0;
    ab.totalBytes = bufferSize;
    ab.au = &au;

    status = GetAudioUnit(&au, fileRenderer, &ab);
  }
  else
  {
    status = GetAudioUnit(&au, sinRenderer, &renderPhase);
  }

  printf("AudioUnit search result: %d\n", status);

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
