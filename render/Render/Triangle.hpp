#ifndef RENDER_TRIANGLE_HPP
#define RENDER_TRIANGLE_HPP

#include "Geo/Vector.hpp"
#include "Draw/Framebuffer.hpp"
#include "Draw/Color.hpp"

namespace Render {
	class Triangle
	{
	public:
		static void render(Draw::Framebuffer &framebuffer, const Geo::Vector &p0, const Geo::Vector &p1, const Geo::Vector &p2, const Draw::Color &color);
	};
}
#endif