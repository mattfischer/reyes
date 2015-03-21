#ifndef PATCH_HPP
#define PATCH_HPP

#include "Geo/Vector.hpp"

#include "Grid.hpp"

#include <memory>

class Patch
{
public:
	Patch(Geo::Vector points[16]);
	Patch(Patch &&other);

	const Geo::Vector &point(int x, int y) const;

	Grid tesselate(int divisions) const;

private:
	std::unique_ptr<Geo::Vector[]> mPoints;
};

#endif