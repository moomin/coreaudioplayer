#define CHUNK_SIZE 512
#define WAV_HEADER_LENGTH 44

int feedTheBuffer(const char *filename, wavBuffer *wav);
int readFile(FILE *stream, void *buffer, size_t bytesToRead);

