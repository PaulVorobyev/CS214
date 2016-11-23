#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include "compressR_worker_LOLS.h"
#include "compressR_LOLS.h"

void compressR_LOLS(char* fname, int parts) {
    void** parsed_filedata = (void**)malloc(sizeof(void*)*3);
    filedata_parser(fname, parsed_filedata);
    char* filename = (char*)parsed_filedata[0];
    char* ext = (char*)parsed_filedata[1];
    int size = (int)(intptr_t)parsed_filedata[2];

    int blocksize = size/parts;
    int endblocksize = blocksize+(size % parts);
    if (parts == 1) {
        endblocksize = size;
    } else if (parts > size) {
        blocksize = 1;
    }
    int sbyte = 0;
    int i;
    for (i = 0; i < parts-1; i++) {
        if (size < blocksize) {
            blocksize = size;
        }
        metadata* md = formMetadata(filename, ext, i, sbyte, blocksize);
        sbyte += blocksize;
        size -= blocksize;
        int pid = fork();
        if (pid == -1) {
            // failed to fork
        } else if (pid == 0) {
            // runs compressPart and then dies
            compressor_worker(md);
            exit(0); // killing children
        }
    }
    if (parts == 1) {
        i = -1;
    }
    if (endblocksize > size) {
        endblocksize = size;
    }
    metadata* md = formMetadata(filename, ext, i, sbyte, endblocksize);
    int pid = fork();
    if (pid == -1) {
        // fork failed
    } else if (pid == 0) {
        compressor_worker(md);
        exit(0);
    }

    // waiting for death of all children
    wait(NULL);
    free(parsed_filedata);
}

int main(int argc, char ** argv) {
    errorCheck(argc, argv);

    compressR_LOLS(argv[1], atoi(argv[2]));
    return 0;
}
