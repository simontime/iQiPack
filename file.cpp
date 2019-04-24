#define _CRT_SECURE_NO_WARNINGS

#include "file.h"

void File::CreateRecursiveDirectories(char *path)
{
	for (char *p = strchr(path + 1, '/');
		p; p = strchr(p + 1, '/')) {
		*p = '\0';
		MKDIR(path);
		*p = '/';
	}
}

void File::DecryptAsset(const char *name, u8 *data, const u32& len)
{
	for (u32 i = 0; i < len; i += 0x2000) {
		u32 remainder = len - i < 0x2000 ? len - i : 0x2000;
		XXTEA::Crypt((u32 *)&data[i], remainder / 4, Utils::GenerateKey(name, len, i));
	}
}

void File::ExtractPack(const char *name, const char *path)
{
	MKDIR(path);
	u32 pathLen = std::strlen(path);

	FILE *in = fopen(name, "rb");

	PACKHeader header;
	fread(&header, sizeof(PACKHeader), 1, in);

	u8 *buf = new u8[header.Size1];
	fread(buf, 1, header.Size1, in);

	DecryptAsset("header", buf, header.Size1);

	if (header.Magic != 'PACK') {
		std::cout << "Error: Invalid PAK file!" << std::endl;
		return;
	}

	u32 numAssets = *(u32 *)buf;
	buf += sizeof(u32);

	while (numAssets--)
	{
		u32 lenStr = *(u32 *)buf;
		buf += sizeof(u32);
		
		char *name = new char[lenStr + pathLen + 2];
		char *hashname = new char[lenStr + 1];

		std::memcpy(hashname, buf, lenStr);
		hashname[lenStr] = 0;

		sprintf(name, "%s/", path);

		memcpy(name + pathLen + 1, buf, lenStr);
		name[lenStr + pathLen + 1] = 0;

		buf += lenStr;

		u32 size1 = *(u32 *)buf;
		buf += sizeof(u32);

		u32 size2 = *(u32 *)buf;
		buf += sizeof(u32);

		u32 offset = *(u32 *)buf;
		buf += 0x14;

		fseek(in, sizeof(PACKHeader) + header.Size1 + offset, SEEK_SET);

		std::cout << "Extracting " << name << "..." << std::endl;

		CreateRecursiveDirectories(name);

		FILE *out = fopen(name, "wb");

		u8 *fbuf = new u8[size1];

		fread(fbuf, 1, size1, in);

		DecryptAsset(hashname, fbuf, size1);

		fwrite(fbuf, 1, size1, out);
		fclose(out);

		delete name;
		delete fbuf;
	}

	std::cout << std::endl << "Done!" << std::endl;
}
