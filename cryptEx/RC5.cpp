#include "RC5_Prefs.h"
#include <math.h>
#include <algorithm>


using namespace std;



void RC5_encrypt(RC5_WORD* data)
{
	RC5_WORD i, A = data[0] + S[0], B = data[1] + S[1];

	for (i = 1; i <= R; i++)
	{
		A = ROTL(A ^ B, B) + S[2 * i];
		B = ROTL(B ^ A, A) + S[2 * i + 1];
	}
	data[0] = A; data[1] = B;
}

void RC5_decrypt(RC5_WORD* data) /* 2 WORD input ct/output pt    */
{
	RC5_WORD i, B = data[1], A = data[0];
	for (i = R; i > 0; i--)
	{
		B = ROTR(B - S[2 * i + 1], A) ^ A;
		A = ROTR(A - S[2 * i], B) ^ B;
	}
	data[1] = B - S[1]; data[0] = A - S[0];
}

void RC5_Init()
{
	RC5_WORD i, L[c];
	//key alignment
	for (i = 0, L[i / u] = 0; i < b; i++)
		L[i / u] = (L[i / u] << 8) + key[i];
	//key extention
	S[0] = P;
	for (i = 1; i < t; i++)
		S[i] = S[i - 1] + Q;
	//key shuffle
	RC5_WORD G, H, j, k;
	G = H = j = i = 0;

	for (k = 0; k < max(3 * c, 3 * 2 * (R + 1)); k++)
	{
		G = S[i] = (S[i] + G + H) << 3;
		H = L[j] = (L[j] + G + H) << (G + H);
		i = (i + 1) % (2 * (R + 1));
		j = (j + 1) % c;
	}

}
