#include "Render/Primitive.hpp"

#include <algorithm>

namespace Render {
	void Primitive::render(const Config &config) const
	{
		Grid grid = dice(config);

		for(int x = 0; x < grid.width(); x++) {
			for(int y = 0; y < grid.height(); y++) {
				Geo::Vector u = grid.point(x + 1, y + 1) - grid.point(x, y);
				Geo::Vector v = grid.point(x, y + 1) - grid.point(x + 1, y);
				Geo::Vector normal = u % v;
				normal.setW(0);
				normal = normal.normalize();
				float l = std::max(normal * (Geo::Vector(1, 1, -1, 0).normalize()), 0.0f);
				grid.setColor(x, y, Draw::Color(0xff, 0x0, 0x0) * l);
			}
		}

		for(int x = 0; x <= grid.width(); x++) {
			for(int y = 0; y <= grid.height(); y++) {
				grid.setPoint(x, y, config.viewport() * (config.projection() * grid.point(x, y)).project());
			}
		}

		grid.render(config);
	}
}