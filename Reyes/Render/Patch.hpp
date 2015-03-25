#ifndef PATCH_HPP
#define PATCH_HPP

#include "Geo/Vector.hpp"

#include "Render/Grid.hpp"
#include "Render/Primitive.hpp"

#include "Draw/Color.hpp"

#include <memory>

namespace Render {
	class Patch : public Primitive
	{
	public:
		Patch(Geo::Vector points[16]);
		Patch(Patch &&other);

		const Geo::Vector &point(int x, int y) const;

		Grid dice(const Config &config) const;

	private:
		std::unique_ptr<Geo::Vector[]> mPoints;
	};
}

#endif