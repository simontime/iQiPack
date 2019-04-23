#include "crypto.h"

u32 Utils::Hash(const char *string, int len)
{
	u32 hash = 0x1505; // Initial hash value.

	for (int i = 0; i < len; i++) /* Perform hash loop for length of string. */ {
		hash += hash << 5; // Faster way of doing hash *= 33.
		hash ^= string[i]; // XOR the current state with a character from the string.
	}

	return hash; // Return the computed hash.
}

u32 *Utils::GenerateKey(const char *string, const int& length, const int& offset)
{
	u32 *keyBuf = new u32[4]; // Allocate key buffer.

	int lenStr = std::strlen(string); // Get length of input string.

	u32 base = 
		(length ^ offset) ^ Hash(string, lenStr); // Generate our key base.

	keyBuf[0] = base; // Duplicate
	keyBuf[1] = base; // it
	keyBuf[2] = base; // 4
	keyBuf[3] = base; // times.

	*(u64 *)&keyBuf[0] &= *(u64 *)&FixedKey[0]; // AND our key buffer
	*(u64 *)&keyBuf[2] &= *(u64 *)&FixedKey[8]; // with the fixed key.

	return keyBuf; // Return the computed key.
}

#define MX (((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4)) \
			^ ((sum ^ y) + (key[(p & 3) ^ e] ^ z)))

void XXTEA::Crypt(u32 *data, int n, u32 *key) {
	u32 y, z, sum, p, rounds, e;

	if (n > 0)
	{
		rounds = 6 + (52 / n);
		sum = rounds * Delta;

		y = data[0];

		do
		{
			e = (sum >> 2) & 3;

			for (p = n - 1; p > 0; p--)
			{
				z = data[p - 1];
				y = data[p] -= MX;
			}

			z = data[n - 1];
			y = data[0] -= MX;

			sum -= Delta;
		}
		while (--rounds);
	}
}