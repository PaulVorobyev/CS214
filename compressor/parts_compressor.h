#ifndef __PART_COMPRESS
#define __PART_COMPRESS

typedef struct metadata {
    char* filename;
    char* ext;
    int part;
    int sbyte;
    int blocksize;
} metadata;

metadata* formMetadata(char*,char*,int,int,int);
void* compressPart(void*);
void filedata_parser(char*,void**);

#endif
