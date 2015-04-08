#ifndef RENDER_TRIANGLE_HPP
#define RENDER_TRIANGLE_HPP

#include "Geo/Vector.hpp"
#include "Draw/Framebuffer.hpp"
#include "Draw/Color.hpp"

namespace Render {
	class Rasterize
	{
	public:
		static void renderQuad(Draw::Framebuffer &framebuffer, const Geo::Vector &p0, const Draw::Color &c0, Geo::Vector &p1, const Draw::Color &c1, Geo::Vector &p2, const Draw::Color &c2, Geo::Vector &p3, const Draw::Color &c3, bool draw123);
	};
}
#endif