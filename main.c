#include <stdio.h>
#include "audio_unit.h"

int main(int argc, char **argv)
{
  int status;
  AudioUnit au;

  status = GetAudioUnit(&au);
  printf("Search result: %d\n", status);

  status = startPlay(&au);
  printf("startPlay: %d\n", status);

  status = stopPlay(&au);
  printf("stopPlay: %d\n", status);

  closeUnit(&au);

  return status;
}

