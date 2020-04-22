extern "C" {
    extern  void* compress(const unsigned char *pSrcData,   unsigned int scrSize, unsigned int *pOutSize);
    extern void freeCompressData(void* pOutData);
} 