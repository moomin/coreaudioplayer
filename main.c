#include <stdio.h>
#include "render_callback.h"
#include "audio_unit.h"
#include "read_file.h"

#define BUFFER_CAPACITY_KB 1024

int playWavFile(char *filename);

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Usage: cap <filename>\n");
    exit(1);
  }

  return playWavFile(argv[1]);
}

int playWavFile(char *filename)
{
  int status;
  AudioUnit au;
  wavHeader header;

  if (getAudioUnit(&au) != noErr)
  {
    fprintf(stderr, "Cannot obtain AudioUnit component\n");
    return 1;
  }

  if (readWavHeader(filename, &header) != 0)
  {
    return 1;
  }

  //setup audioBuffer info for renderer
  pcmBuffer pcm;
  pcm.bufferCapacity = BUFFER_CAPACITY_KB * 1024;
  pcm.startPtr = calloc(BUFFER_CAPACITY_KB, 1024);
  pcm.boundaryPtr = pcm.startPtr + (BUFFER_CAPACITY_KB * 1024 / 2);
  pcm.currentPtr = pcm.startPtr;
  pcm.bytesLeftA = 0;
  pcm.bytesLeftB = 0;
  pcm.header = header;

  if (setupAudioUnit(&au, pcmRenderer, &pcm) != noErr)
  {
    closeUnit(&au);
    fprintf(stderr, "Error setting up an AudioUnit. Exiting\n");
    return 1;
  }

  printf("starting playback\n");
  status = startPlay(&au);

  feedTheBuffer(filename, &pcm);

  printf("stopping playback\n");
  status = stopPlay(&au);

  closeUnit(&au);

  return status;
}

