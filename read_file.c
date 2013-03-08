#include <stdio.h>
#include <string.h>
#include <time.h>
#include "render_callback.h"
#include "read_file.h"

int feedTheBuffer(const char *filename, wavBuffer *wav)
{
  size_t bytesRead;
  FILE *stream;
  stream = fopen(filename, "r");
  void *bufferPtr;
  size_t halfBufferCapacity;

  halfBufferCapacity = wav->bufferCapacity / 2;

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
    if (wav->bytesLeftA == 0)
    {
      bytesRead = readFile(stream, wav->startPtr, halfBufferCapacity);
      wav->bytesLeftA = bytesRead;
    }

    if (wav->bytesLeftB == 0)
    {
      bytesRead = readFile(stream, wav->boundaryPtr, halfBufferCapacity);
      wav->bytesLeftB = bytesRead;
    }

    nanosleep(&pause, NULL);
  }
  while(bytesRead > 0);

  fclose(stream);

  return bytesRead;
}

int readWavHeader(const char *filename, streamFormat *fmt)
{
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

