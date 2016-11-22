#include "LOLS.h"
#include <stdlib.h>
#include <string.h>

// the compression algorithm
int power(int a, int b) {
    int r = 1;
    while (b != 0) {
        r *= a;
        b--;
    }
    return r;
}

char* int_to_str(int i) {
    int mul = 10;
    int e = 1;
    int cpy = i;
    cpy /= 10;
    while (cpy > 0) { // gets length
        cpy /= 10;
        e += 1;
    }
    char* strI = (char*)malloc(sizeof(char)*(e+1));
    strI[e] = '\0'; // appending the null terminator
    int j = e-1;
    // fill it up!
    while (i != 0) {
        char v = i % 10;
        i /= 10;
        strI[j] = (v + '0');
        j--;
    }
    return strI;
}

// destructive
void compressor(char* raw) {
    int len = strlen(raw);
    char* compressed = (char*)malloc(sizeof(char)*(len+1));
    char tgt = raw[0];
    int cnt = 1;
    int i, ci;
    ci = 0;
    for (i = 1; i <= len; i++) {
        char cc = raw[i];
        if (cc != tgt) {
            if (cnt == 1) {
                compressed[ci] = tgt;
                ci += 1;
            } else if (cnt == 2) {
                compressed[ci] = compressed[ci+1] = tgt;
                ci += 2;
            } else {
                char* strI = int_to_str(cnt);
                memcpy(compressed+ci, strI, strlen(strI));
                ci += strlen(strI);
                free(strI);
                compressed[ci] = tgt;
                ci += 1;
            }
            tgt = cc;
            cnt = 0;
        }
        cnt += 1;
    }
    compressed[ci] = '\0';
    memcpy(raw, compressed, ci+1);
}
