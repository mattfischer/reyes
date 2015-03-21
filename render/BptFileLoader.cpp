#include "BptFileLoader.hpp"

#include <fstream>

PatchSet BptFileLoader::load(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	int numPatches;

	file >> numPatches;
	std::vector<Patch> patches;
	for(int i = 0; i < numPatches; i++) {
		int dimx, dimy;
		file >> dimx >> dimy;
		Geo::Vector points[16];
		for(int j = 0; j < 16; j++) {
			float x, y, z;
			file >> x >> y >> z;
			points[j] = Geo::Vector(x, y, z);
		}
		patches.push_back(Patch(points));
	}

	return PatchSet(std::move(patches));
}