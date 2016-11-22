#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include "parts_compressor.h"
#include "compressT_LOLS.h"

void compressT_LOLS(char* fname, int parts) {
    pthread_t* pool = (pthread_t*)malloc(sizeof(pthread_t)*parts);
    int i;
    void** parsed_filedata = (void**)malloc(sizeof(void*)*3);
    filedata_parser(fname, parsed_filedata);
    char* filename = (char*)parsed_filedata[0];
    char* ext = (char*)parsed_filedata[1];
    int size = (int)(intptr_t)parsed_filedata[2];
    int blocksize = size/parts;
    int endblocksize = blocksize+(size % parts); // block size of last read
    if (parts == 1) { // if single thread, endblocksize is
        endblocksize = size;
    // if parts is greater than the size of file, each thread processes a single character
    } else if (parts > size) {
        blocksize = 1;
    }
    int sbyte = 0;
    for (i = 0; i < parts-1; i++) {
        if (size < blocksize) {
            blocksize = size;
        }
        metadata* md = formMetadata(filename, ext, i, sbyte, blocksize);
        pthread_create(pool+i, NULL, compressPart, (void*)md);
        sbyte += blocksize;
        size -= blocksize;
    }
    if (parts == 1) {
        i = -1;
    }
    if (endblocksize > size) {
        endblocksize = size;
    }
    metadata* md = formMetadata(filename, ext, i, sbyte, endblocksize);
    pthread_create(pool+parts-1, NULL, compressPart, (void*)md);
    for (i = 0; i < parts; i++) {
        pthread_join(pool[i], NULL);
    }
    free(parsed_filedata);
}

int main(int argc, char** argv) {
    compressT_LOLS("test.txt", 3);
    return 0;
}
