#include <stdio.h>
#include <string.h>
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
    return 1;
  }

  //skip WAV header
  fseek(stream, WAV_HEADER_LENGTH, SEEK_SET);


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

    //sleep for 2s
    usleep(2000 * 1000);
  }
  while(bytesRead > 0);

  fclose(stream);

  return bytesRead;
}

int readFile(FILE *stream, void *buffer, size_t bytesToRead)
{
  char *ioBuffer[CHUNK_SIZE * sizeof(int)];
  size_t itemsRead;

  itemsRead = bytesToRead * fread(buffer, bytesToRead, 1, stream);

  if (itemsRead == bytesToRead)
  {
    return itemsRead;
  }

  return fread(buffer, 1, bytesToRead, stream);
}

