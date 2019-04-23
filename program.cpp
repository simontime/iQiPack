#include "file.h"

int main(int argc, char **argv)
{
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " *.pak" << std::endl;
		return 1;
	}
	File::ExtractPack(argv[1]);
	return 0;
}