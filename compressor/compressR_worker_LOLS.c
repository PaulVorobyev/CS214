#include <stdlib.h>
#include <stdio.h>
#include "parts_compressor.h"

void compressor_worker(metadata* md) {
    compressPart((void*)md);
}
