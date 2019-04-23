#define _CRT_SECURE_NO_WARNINGS

#include "file.h"

void File::CreateRecursiveDirectories(char *path)
{
	for (char *p = std::strchr(path + 1, '/');
		p; p = std::strchr(p + 1, '/')) {
		*p = '\0';
		MKDIR(path);
		*p = '/';
	}
}

void File::DecryptAsset(const char* name, u8 *data, const int& len)
{
	for (int i = 0; i < len; i += 0x2000) {
		int remainder = len - i < 0x2000 ? len - i : 0x2000;
		XXTEA::Crypt((u32 *)&data[i], remainder / 4, Utils::GenerateKey(name, len, i));
	}
}

void File::ExtractPack(const char *name)
{
	FILE *in = fopen(name, "rb");

	PACKHeader header;
	fread(&header, sizeof(PACKHeader), 1, in);

	u8 *buf = new u8[header.Size1];
	fread(buf, 1, header.Size1, in);

	DecryptAsset("header", buf, header.Size1);

	if (header.Magic != 'KCAP') {
		std::cout << "Error: Invalid PAK file!" << std::endl;
		return;
	}

	int numAssets = *(u32 *)buf;
	buf += sizeof(u32);

	for (int i = 0; i < numAssets; i++)
	{
		u32 lenStr = *(u32 *)buf;
		buf += sizeof(u32);

		char *name = new char[lenStr + 1];
		std::memcpy(name, buf, lenStr);
		name[lenStr] = 0;
		buf += lenStr;

		int size1 = *(u32 *)buf;
		buf += sizeof(u32);

		int size2 = *(u32 *)buf;
		buf += sizeof(u32);

		int offset = *(u32 *)buf;
		buf += 0x14;

		fseek(in, sizeof(PACKHeader) + header.Size1 + offset, SEEK_SET);

		std::cout << "Extracting " << name << "..." << std::endl;

		CreateRecursiveDirectories(name);

		FILE *out = fopen(name, "wb");

		u8 *fbuf = new u8[size1];

		fread(fbuf, 1, size1, in);

		DecryptAsset(name, fbuf, size1);

		fwrite(fbuf, 1, size1, out);
		fclose(out);

		delete name;
		delete fbuf;
	}

	delete buf;
}
