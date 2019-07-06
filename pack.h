#pragma once

#include <iostream>
#include <Windows.h>

#include "crypto.h"
#include "types.h"

#define MKDIR(name) (CreateDirectoryA(name, NULL))

struct Header 
{
	u32 Magic;
	u32 Version;
	char Unk1[0xC];
	u32 Size1;
	u32 Size2;
	char Unk2[0xC];
};

class Pack
{
private:
	const char *inputFile;

	void CreateRecursiveDirectories(char *dirName);
	void DecryptAsset(const char *name, u8 *data, const u32 length);

public:
	Pack(const char *inputFile);
	void Extract(const char *outputDir);
};