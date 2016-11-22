#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "parts_compressor.h"
#include "LOLS.h"

// helps to generate metadata
metadata* formMetadata(char* filename, char* ext, int part, int sbyte, int blocksize) {
    metadata* md = (metadata*)malloc(sizeof(metadata));
    md->filename = filename;
    md->ext = ext;
    md->part = part;
    md->sbyte = sbyte;
    md->blocksize = blocksize;
    return md;
}

// handles the compression and writing to a part file
void* compressPart(void* args) { // filename, index, sbyte, blocksize
    metadata* md = (metadata*)args;
    int fnamesize = strlen(md->filename);
    int extsize = strlen(md->ext);
    // forming input filename
    char* in = (char*)malloc(sizeof(char)*(fnamesize + extsize + 2)); 
    sprintf(in, "%s.%s", md->filename, md->ext);
    in[fnamesize+extsize+1] = '\0';
    FILE* fp = fopen(in, "r");
    fseek(fp, md->sbyte, SEEK_SET);
    // allocating block and fill
    char* block = (char*)malloc(sizeof(char)*(md->blocksize+1));
    block[md->blocksize] = '\0';
    fread(block, sizeof(char), md->blocksize, fp);
    fclose(fp);
    free(in);
    char* out;
    // forming output filename
    if (md->part == -1) {
        out = (char*)malloc(sizeof(char)*(fnamesize+extsize+8));
        sprintf(out, "%s_%s_LOLS", md->filename, md->ext);
        out[fnamesize+extsize+7] = '\0';
    } else {
        out = (char*)malloc(sizeof(char)*(fnamesize+extsize+20));
        snprintf(out, fnamesize+extsize+20, "%s_%s_LOLS%d", md->filename, md->ext, md->part);
    }
    fp = fopen(out, "w"); // opening file for writing
    compressor(block);
    printf("%s %d\n", block, md->part);
    fwrite(block, sizeof(char), strlen(block), fp);
    fclose(fp);
    free(out);
    free(args);
    return NULL;
}

void filedata_parser(char* fname, void** store) {
    int i;
    // separating the filename and extension
    int namelen = strlen(fname);
    for (i = namelen-1; i >= 0; i--) {
        if (fname[i] == '.') {
            break;
        }
    }
    char* filename = (char*)malloc(sizeof(char)*(i+1));
    char* ext = (char*)malloc(sizeof(char)*(namelen-i+1));
    filename[i] = '\0';
    ext[namelen-i] = '\0';
    memcpy(filename, fname, i);
    memcpy(ext, fname+i+1, namelen-i);
    // getting file size
    FILE* fp = fopen(fname, "r");
    fseek(fp, 0L, SEEK_END);
    int size = ftell(fp);
    fclose(fp);
    store[0] = (void*)filename;
    store[1] = (void*)ext;
    store[2] = (void*)(intptr_t)size;
}
