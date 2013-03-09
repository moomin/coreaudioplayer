#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "render_callback.h"
#include "read_file.h"

int feedTheBuffer(const char *filename, pcmBuffer *pcm)
{
  size_t bytesRead;
  FILE *stream;
  stream = fopen(filename, "r");
  void *bufferPtr;
  size_t halfBufferCapacity;

  halfBufferCapacity = pcm->bufferCapacity / 2;

  if (stream == NULL)
  {
    printf("Cannot open file: %s\n", filename);
    return 1;
  }

  //skip WAV header
  fseek(stream, WAV_HEADER_LENGTH, SEEK_SET);

  struct timespec pause;
  pause.tv_sec = 2;
  pause.tv_nsec = 0;

  do
  {
    if (pcm->bytesLeftA == 0)
    {
      bytesRead = readFile(stream, pcm->startPtr, halfBufferCapacity);
      pcm->bytesLeftA = bytesRead;
    }

    if (pcm->bytesLeftB == 0)
    {
      bytesRead = readFile(stream, pcm->boundaryPtr, halfBufferCapacity);
      pcm->bytesLeftB = bytesRead;
    }

    nanosleep(&pause, NULL);
  }
  while(bytesRead > 0);

  fclose(stream);

  return bytesRead;
}

int readWavHeader(const char *filename, wavHeader *header)
{
  FILE *stream;
  stream = fopen(filename, "r");

  if (stream == NULL)
  {
    printf("Cannot open file: %s\n", filename);
    return 1;
  }

  if (fread(header, sizeof(*header), 1, stream) == 0)
  {
    printf("Cannot read WAV header\n");
    return 2;
  }


  if (strncmp(header->chunkId, "RIFF", 4) != 0)
  {
    printf("RIFF header not found\n");
    return 3;
  }

  if (strncmp(header->format, "WAVE", 4) != 0)
  {
    printf("Format of the file is not WAVE\n");
    return 4;
  }

  if (header->audioFormat != 1)
  {
    printf("Audio format is not PCM\n");
    return 5;
  }

  printf("SR: %d\nbits: %d\n", header->sampleRate, header->bitsPerSample);

  fclose(stream);

  return 0;
}

int readFile(FILE *stream, void *buffer, size_t bytesToRead)
{
  size_t itemsRead;

  itemsRead = bytesToRead * fread(buffer, bytesToRead, 1, stream);

  if (itemsRead == bytesToRead)
  {
    return itemsRead;
  }

  return fread(buffer, 1, bytesToRead, stream);
}

