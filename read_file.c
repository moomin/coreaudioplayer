#include <stdio.h>
#include <string.h>
#include "read_file.h"

#define CHUNK_SIZE 512

int readFile(const char *filename, char *buffer, size_t bufferSize)
{
  FILE *stream;
  stream = fopen(filename, "r");

  if (stream == NULL)
  {
    return 1;
  }

  char *ioBuffer[CHUNK_SIZE * 4];
  char *bufferPos = buffer;

  size_t itemsRead, totalBytesRead;

  do
  {
    itemsRead = fread(ioBuffer, CHUNK_SIZE, 4, stream);
    bufferSize -= itemsRead * CHUNK_SIZE;
    memcpy(bufferPos, ioBuffer, itemsRead * CHUNK_SIZE);
    bufferPos += sizeof(char) * CHUNK_SIZE * 4;
  }
  while ((itemsRead > 0) && (bufferSize > 0));

  return fclose(stream);
}

int dumpBuffer(const char *filename, char *buffer, size_t bufferSize)
{
  FILE *stream;
	stream = fopen(filename, "w");

  char *bufferPos = buffer;

  do
  {
    fwrite(bufferPos, CHUNK_SIZE, 4, stream);
    bufferPos += sizeof(char) * CHUNK_SIZE * 4;
    bufferSize -= CHUNK_SIZE * 4;
  }
  while (bufferSize > 0);

  return fclose(stream);
}
