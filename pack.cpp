#define _CRT_SECURE_NO_WARNINGS

#include "pack.h"

Pack::Pack(const std::string inputFile)
{
	this->inputFile = inputFile;
}

void Pack::DecryptAsset(const std::string name, u8 *data, const u32 length)
{
	for (u32 i = 0; i < length; i += 0x2000) // Decrypt in 0x2000 chunks until remaining length in file <0x2000.
	{
		u32 remainder = (length - i < 0x2000) ? length - i : 0x2000;
		XXTEA::Decrypt((u32 *)&data[i], remainder / 4, Utils::GenerateKey(name, length, i));
	}
}

void Pack::Extract(const std::string outputDir)
{
	fs::create_directories(outputDir);

	FILE *in = fopen(inputFile.c_str(), "rb");

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

	u32 numAssets = Read<u32>(&buf); // 0x0-0x4 in the header is the total number of assets within.

	while (numAssets--)
	{
		u32 lenStr = Read<u32>(&buf); // The string is prepended with its length.
		
		std::string name = outputDir + '/', hashname;

		for (u32 i = 0; i < lenStr; i++)
			hashname += *buf++;

		name += hashname;

		fs::create_directories(name.substr(0, name.find_last_of('/'))); // Create recursive directories for the asset's directory name.

		FILE *out = fopen(name.c_str(), "wb");

		u32 size1  = Read<u32>(&buf); // Read the asset size.
		u32 size2  = Read<u32>(&buf); // Read the duplicate of the asset size (why the fuck is this necessary?).
		u32 offset = Read<u32>(&buf); // Read the offset of the asset within the pack.

		SeekForward(&buf, 0x10);

		fseeko64(in, sizeof(Header) + header.Size1 + offset, SEEK_SET); // Seek to its offset.

		std::cout << "Extracting " << name << "..." << std::endl;

		u8 *fbuf = new u8[size1];

		fread(fbuf, 1, size1, in); // Read the asset data into a buffer.
		DecryptAsset(hashname, fbuf, size1); // Decrypt the asset.
		fwrite(fbuf, 1, size1, out); // Write the decrypted asset data into our newly created file.

		fclose(out);

		delete[] fbuf;
	}

	fclose(in);

	std::cout << std::endl << "Done!" << std::endl;
}

template <typename T>
T Pack::Read(u8 **in)
{
	T buf = *reinterpret_cast<T *>(*in);
	SeekForward(in, sizeof(T));
	return buf;
}

void Pack::SeekForward(u8 **in, size_t pos)
{
	*in += pos;
}