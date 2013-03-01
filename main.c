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
      printf("error reading file into buffer\n");
      return 1;
    }

    audioBuffer ab;

    ab.audioDataPos = buffer;
    ab.playedBytes = 0;
    ab.totalBytes = 1024*1024;

    printf("file successfully read into buffer\n");

    dumpBuffer("./buffer.dump", buffer, bufferSize);
    printf("file successfully dumped\n");

    status = GetAudioUnit(&au, fileRenderer, &ab);
  }
  else
  {
    status = GetAudioUnit(&au, sinRenderer, &renderPhase);
  }

  printf("Search result: %d\n", status);

  status = startPlay(&au);
  printf("startPlay: %d\n", status);

  status = stopPlay(&au);
  printf("stopPlay: %d\n", status);

  closeUnit(&au);

  return status;
}

int prepareBuffer(char *filename, char *buffer, size_t bufferSize)
{
  return readFile(filename, buffer, bufferSize);
}
