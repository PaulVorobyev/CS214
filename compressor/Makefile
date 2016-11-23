CC=gcc

outR:
	$(CC) compressR_LOLS.c parts_compressor.c LOLS.c compressR_worker_LOLS.c -o outR

outT:
	$(CC) LOLS.c parts_compressor.c compressT_LOLS.c -o outT
	rm -rf outT.dSYM

clean:
	rm -rf outR outT
