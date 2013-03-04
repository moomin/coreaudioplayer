#include <stdio.h>
#include "render_callback.h"
#include "audio_unit.h"
#include "read_file.h"

#define RENDER_FILE true

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

  void *buffer;
  size_t bufferSize = 1024 * 1024;
  buffer = calloc(1024, 1024);

  if (prepareBuffer(filename, buffer, bufferSize) != 0)
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

  if (status != noErr)
  {
    closeUnit(&au);
    printf("Error setting up an AudioUnit. Exiting\n");
    return 1;
  }

  printf("starting playback\n");
  status = startPlay(&au);

//just enough time to play 1M worth of 44.1kHz 16bit stereo WAV data
  usleep(7000000);

/* just discovered that I don't need to call CFRunLoopInMode
  SInt32 exit_reason;
  do exit_reason = CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0.51, false);
  while (1);
*/

  printf("stopping playback\n");
  status = stopPlay(&au);

  closeUnit(&au);

  return status;
}

int prepareBuffer(char *filename, char *buffer, size_t bufferSize)
{
  return readFile(filename, buffer, bufferSize);
}
