#define _CRT_SECURE_NO_WARNINGS

#include "pack.h"

Pack::Pack(const char *inputFile)
{
	this->inputFile = inputFile;
}

void Pack::CreateRecursiveDirectories(char *dirName)
{
	for (char *p = strchr(dirName + 1, '/'); 
		p; p = strchr(p + 1, '/'))
	{
		*p = '\0';
		MKDIR(dirName);
		*p = '/';
	}
}

void Pack::DecryptAsset(const char *name, u8 *data, const u32 length)
{
	for (u32 i = 0; i < length; i += 0x2000) // Decrypt in 0x2000 chunks until remaining length in file <0x2000.
	{
		u32 remainder = (length - i < 0x2000) ? length - i : 0x2000;
		XXTEA::Decrypt((u32 *)&data[i], remainder / 4, Utils::GenerateKey(name, length, i));
	}
}

void Pack::Extract(const char *outputDir)
{
	MKDIR(outputDir);
	u32 pathLen = strlen(outputDir);

	FILE *in = fopen(inputFile, "rb");

	Header header;
	fread(&header, sizeof(Header), 1, in);

	u8 *buf = new u8[header.Size1];
	fread(buf, 1, header.Size1, in);

	DecryptAsset("header", buf, header.Size1); // Decrypt the header using the string "header".

	if (header.Magic != 'PACK')
	{
		std::cout << "Error: Invalid PAK file!" << std::endl;
		return;
	}

	u32 numAssets = *(u32 *)buf; // 0x0-0x4 in the header is the total number of assets within.
	buf += sizeof(u32);

	while (numAssets--)
	{
		u32 lenStr = *(u32 *)buf; // The string is prepended with its length.
		buf += sizeof(u32);
		
		char *name = new char[lenStr + pathLen + 2];
		char *hashname = new char[lenStr + 1];

		memcpy(hashname, buf, lenStr); // Read the string.
		hashname[lenStr] = 0;

		sprintf(name, "%s/", outputDir);

		memcpy(name + pathLen + 1, buf, lenStr); // Copy the filename into a second buffer for the output name.
		name[lenStr + pathLen + 1] = 0;

		buf += lenStr;

		u32 size1 = *(u32 *)buf; // Read the asset size.
		buf += sizeof(u32);

		u32 size2 = *(u32 *)buf; // Read the duplicate of the asset size (why the fuck is this necessary?).
		buf += sizeof(u32);

		u32 offset = *(u32 *)buf; // Read the offset of the asset within the pack.
		buf += 0x14;

		_fseeki64(in, sizeof(Header) + header.Size1 + offset, SEEK_SET); // Seek to its offset.

		std::cout << "Extracting " << name << "..." << std::endl;

		CreateRecursiveDirectories(name); // Create recursive directories for the assets directory name.

		FILE *out = fopen(name, "wb");

		u8 *fbuf = new u8[size1];

		fread(fbuf, 1, size1, in); // Read the asset data into a buffer.

		DecryptAsset(hashname, fbuf, size1); // Decrypt the asset.

		fwrite(fbuf, 1, size1, out); // Write the decrypted asset data into our newly created file.
		fclose(out);

		delete name;
		delete fbuf;
	}

	std::cout << std::endl << "Done!" << std::endl;
}
