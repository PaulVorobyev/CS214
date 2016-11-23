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
void fail(const char*);
int exists(const char*);
void errorCheck(int, char **);
void* compressPart(void*);
void filedata_parser(char*,void**);

#endif
