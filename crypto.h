#pragma once

#include <iostream>

#include "common.h"

#define MIX (((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4)) \
			^ ((sum ^ y) + (key[(p & 3) ^ e] ^ z)))

constexpr u32 Delta = 0x9E3779B9;
constexpr u32 FixedKey[] = { 0xA0D0FFB0, 0x81230089, 0x12159842, 0xFF78F3C7 };

namespace Utils
{
	u32 Hash(const std::string string);
	u32 *GenerateKey(const std::string string, const u32 length, const u32 offset);
}

namespace XXTEA
{
	void Decrypt(u32 *data, const u32 n, const u32 *key);
}
