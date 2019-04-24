#include "file.h"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: " << argv[0] << " <*.pak> <output directory>" << std::endl;
		return 1;
	}

	File::ExtractPack(argv[1], argv[2]);

	return 0;
}