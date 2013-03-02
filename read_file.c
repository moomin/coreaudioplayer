#include <stdio.h>
#include <string.h>
#include "read_file.h"

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

  //skip WAV header
  fseek(stream, WAV_HEADER_LENGTH, SEEK_SET);

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
