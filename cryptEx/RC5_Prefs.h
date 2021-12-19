

typedef unsigned long RC5_WORD;

#define w		32 // word size in bits
#define R		12 // rounds
#define b		16 // key size in bytes
const int c = 8 * b / w; // words in key
const int t = 2 * (R + 1); //expanded key table size
const int u = w / 8; // word size in bytes
unsigned char key[b] = "727-1-Antipov"; // key as such
RC5_WORD S[t]; // expanded key table
RC5_WORD P = 0xb7e15163, Q = 0x9e3779b9; // magic constants

#define ROTL(x,y) (((x)<<(y&(w-1))) | ((x)>>(w-(y&(w-1)))))
#define ROTR(x,y) (((x)>>(y&(w-1))) | ((x)<<(w-(y&(w-1)))))