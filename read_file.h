int feedTheBuffer(const char *filename, pcmBuffer *pcm);
int readWavHeader(const char *filename, wavHeader *header);
int readFile(FILE *stream, void *buffer, size_t bytesToRead);

