#ifndef RENDER_TRIANGLE_HPP
#define RENDER_TRIANGLE_HPP

#include "Geo/Vector.hpp"
#include "Draw/Framebuffer.hpp"
#include "Draw/Color.hpp"

namespace Render {
	class Rasterize
	{
	public:
		static void renderTriangle(Draw::Framebuffer &framebuffer, const Geo::Vector &p0, const Draw::Color &c0, Geo::Vector &p1, const Draw::Color &c1, Geo::Vector &p2, const Draw::Color &c2);
	};
}
#endif