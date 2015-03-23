#ifndef PATCH_HPP
#define PATCH_HPP

#include "Geo/Vector.hpp"

#include "Render/Grid.hpp"
#include "Render/Object.hpp"

#include "Draw/Color.hpp"

#include <memory>

namespace Render {
	class Patch : public Object
	{
	public:
		Patch(Geo::Vector points[16], const Draw::Color &color);
		Patch(Patch &&other);

		const Geo::Vector &point(int x, int y) const;

		virtual void render(const Config &config) const;

	private:
		Grid tesselate(const Config &config, int divisions) const;

		std::unique_ptr<Geo::Vector[]> mPoints;
		Draw::Color mColor;
	};
}

#endif