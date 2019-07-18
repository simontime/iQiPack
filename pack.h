#pragma once

#include "crypto.h"

#if _WIN32
#define fseeko64 _fseeki64
#endif

namespace fs = std::experimental::filesystem;

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
	std::string inputFile;

	void CreateRecursiveDirectories(const std::string dirName);
	void DecryptAsset(const std::string name, u8 *data, const u32 length);
	template <typename T> T Read(u8 **in);
	void SeekForward(u8 **in, size_t pos);

public:
	Pack(const std::string inputFile);
	void Extract(const std::string outputDir);
};