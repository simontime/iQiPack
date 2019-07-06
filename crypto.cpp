#include "crypto.h"

namespace Utils
{
	u32 Hash(const char *string, const u32 length)
	{
		u32 hash = 0x1505; // Initial hash value.

		for (u32 i = 0; i < length; i++) // Perform hash loop for length of string.
		{
			hash = hash * 33;  // Multiply hash by 33.
			hash ^= string[i]; // XOR the current state with a character from the string.
		}

		return hash; // Return the computed hash.
	}

	u32 *GenerateKey(const char *string, const u32 length, const u32 offset)
	{
		u32 keyBuf[4]; // Allocate key buffer.

		u32 base = (length ^ offset) ^ Hash(string, strlen(string)); // Generate our key base.

		for (int i = 0; i < 4; i++) // Duplicate it 4 times, ANDing the key buffer with the fixed key.
			keyBuf[i] = base & FixedKey[i]; 

		return keyBuf; // Return the computed key.
	}
}

namespace XXTEA
{
	void Decrypt(u32 *data, const u32 n, const u32 *key)
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
}
