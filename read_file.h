#define CHUNK_SIZE 512
#define WAV_HEADER_LENGTH 44

int feedTheBuffer(const char *filename, pcmBuffer *pcm);
int readWavHeader(const char *filename, wavHeader *header);
int readFile(FILE *stream, void *buffer, size_t bytesToRead);

