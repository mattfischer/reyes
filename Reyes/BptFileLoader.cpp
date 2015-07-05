#include "BptFileLoader.hpp"

#include "Object/RenderableObjects.hpp"
#include "Object/Primitives/Patch.hpp"

#include <vector>
#include <fstream>

std::unique_ptr<Object::RenderableObject> BptFileLoader::load(const std::string &filename, Render::Texture &texture)
{
	std::ifstream file(filename.c_str());
	int numPatches;

	file >> numPatches;
	std::vector<std::unique_ptr<const Object::RenderableObject>> patches;
	for(int i = 0; i < numPatches; i++) {
		int dimx, dimy;
		file >> dimx >> dimy;
		Geo::Vector points[16];
		for(int j = 0; j < 16; j++) {
			float x, y, z;
			file >> x >> y >> z;
			points[j] = Geo::Vector(x, y, z);
		}
		std::unique_ptr<Object::Primitive> patch = std::make_unique<Object::Primitives::Patch>(points, texture);
		unsigned int index = patch->newVarying("tex", 3);
		patch->setVaryingVector(index, 0, Geo::Vector(0, 0, 0));
		patch->setVaryingVector(index, 1, Geo::Vector(1, 0, 0));
		patch->setVaryingVector(index, 2, Geo::Vector(0, 0.5f, 0));
		patch->setVaryingVector(index, 3, Geo::Vector(1, 0.5f, 0));
		patches.push_back(std::move(patch));
	}

	return std::make_unique<Object::RenderableObjects>(std::move(patches));
}