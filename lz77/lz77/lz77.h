#ifndef LZ77_H
#define LZ77_H

int lz77Compress(istream &in, int inputFileSize,  ostream &out);
int lz77Decompress(istream &in, ostream &out);

#endif
