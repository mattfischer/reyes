#include "BptFileLoader.hpp"

#include "Render/Objects.hpp"
#include "Render/Patch.hpp"

#include <vector>
#include <fstream>

std::unique_ptr<Render::Object> BptFileLoader::load(const std::string &filename, const Draw::Color &color)
{
	std::ifstream file(filename.c_str());
	int numPatches;

	file >> numPatches;
	std::vector<std::unique_ptr<Render::Object>> patches;
	for(int i = 0; i < numPatches; i++) {
		int dimx, dimy;
		file >> dimx >> dimy;
		Geo::Vector points[16];
		for(int j = 0; j < 16; j++) {
			float x, y, z;
			file >> x >> y >> z;
			points[j] = Geo::Vector(x, y, z);
		}
		std::unique_ptr<Render::Patch> patch = std::make_unique<Render::Patch>(points);
		unsigned int index = patch->newUniform("color", 3);
		patch->setUniform(index + 0, color.r);
		patch->setUniform(index + 1, color.g);
		patch->setUniform(index + 2, color.b);
		patches.push_back(std::move(patch));
	}

	return std::make_unique<Render::Objects>(std::move(patches));
}