#ifndef PATCH_HPP
#define PATCH_HPP

#include "Geo/Vector.hpp"

#include "Grid.hpp"
#include "RenderObject.hpp"

#include <memory>

class Patch : public RenderObject
{
public:
	Patch(Geo::Vector points[16]);
	Patch(Patch &&other);

	const Geo::Vector &point(int x, int y) const;

	virtual void render(const RenderConfig &config) const;

private:
	Grid tesselate(const RenderConfig &config, int divisions) const;

	std::unique_ptr<Geo::Vector[]> mPoints;
};

#endif