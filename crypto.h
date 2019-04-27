#pragma once

#include <iostream>

#include "types.h"

#define MIX (((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4)) \
			^ ((sum ^ y) + (key[(p & 3) ^ e] ^ z)))

constexpr u32 Delta = 0x9E3779B9;

constexpr u8 FixedKey[] = 
{ 
	0xB0, 0xFF, 0xD0, 0xA0,
	0x89, 0x00, 0x23, 0x81,
	0x42, 0x98, 0x15, 0x12,
	0xC7, 0xF3, 0x78, 0xFF
};

class Utils
{
public:
	static u32 Hash(const char *string, const u32 length);
	static u32 *GenerateKey(const char *string, const u32 length, const u32 offset);
};

class XXTEA
{
public:
	static void Decrypt(u32 *data, const u32 n, u32 *key);
};