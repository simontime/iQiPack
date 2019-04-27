#include "crypto.h"

u32 Utils::Hash(const char *string, const u32 length)
{
	u32 hash = 0x1505; // Initial hash value.

	for (u32 i = 0; i < length; i++) // Perform hash loop for length of string.
	{
		hash += hash << 5; // Faster way of doing hash *= 33.
		hash ^= string[i]; // XOR the current state with a character from the string.
	}

	return hash; // Return the computed hash.
}

u32 *Utils::GenerateKey(const char *string, const u32 length, const u32 offset)
{
	u32 *keyBuf = new u32[4]; // Allocate key buffer.

	u32 lenStr = strlen(string); // Get length of input string.

	u32 base =  (length ^ offset) ^ Hash(string, lenStr); // Generate our key base.

	keyBuf[0] = base; // Duplicate
	keyBuf[1] = base; // it
	keyBuf[2] = base; // 4
	keyBuf[3] = base; // times.

	*(u64 *)&keyBuf[0] &= *(u64 *)&FixedKey[0]; // AND our key buffer
	*(u64 *)&keyBuf[2] &= *(u64 *)&FixedKey[8]; // with the fixed key.

	return keyBuf; // Return the computed key.
}

void XXTEA::Decrypt(u32 *data, const u32 n, u32 *key)
{
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
				y = data[p] -= MIX;
			}

			z = data[n - 1];
			y = data[0] -= MIX;

			sum -= Delta;
		}
		while (--rounds);
	}
}