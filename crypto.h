#pragma once

#include <iostream>

#include "types.h"

constexpr u32 Delta = 0x9E3779B9;

constexpr u8 FixedKey[] = { 0xB0, 0xFF, 0xD0, 0xA0, 0x89, 0x00, 0x23, 0x81, 0x42, 0x98, 0x15, 0x12, 0xC7, 0xF3, 0x78, 0xFF };

class Utils
{
public:
	static u32 Hash(const char *, u32);
	static u32 *GenerateKey(const char *, const u32&, const u32& = 0);
};

class XXTEA
{
public:
	static void Crypt(u32 *, u32, u32 *);
};