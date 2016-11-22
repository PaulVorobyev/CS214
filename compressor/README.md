Can you write the make file? we'll prob need a test executable.
Description of files

LOLS
    contains the compression function that makes in a char* array and performs RLE
parts_compressor
    contains functions and structs that are shared between compressR and compressT
compressR_LOLS
    contains the parent of the fork()'d implementation (processes)
compressR_worker_LOLS
    contains the child of the forked()'d implementation
compressT_LOLS
    contains the multithreaded implementation

LOLS - depends on nothing

parts_compressor - depends on LOLS

compressR_worker_LOLS - depends on parts_compressor

compressR_LOLS - depends om compressR_worker_LOLS

compressT_LOLS - depends on parts_compressor
