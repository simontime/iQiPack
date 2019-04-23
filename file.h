#pragma once

#include "crypto.h"
#include "types.h"

#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#define MKDIR(name) (CreateDirectoryA(name, NULL))
#else
#include <sys/stat.h>
#define MKDIR(name) (mkdir(name, 0777))
#endif

struct PACKHeader 
{
	u32 Magic;
	u32 Version;
	char Unk1[0xC];
	u32 Size1;
	u32 Size2;
	char Unk2[0xC];
};

class File
{
public:
	static void CreateRecursiveDirectories(char *);
	static void DecryptAsset(const char *, u8 *, const int&);
	static void ExtractPack(const char *);
};

