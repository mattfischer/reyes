#ifndef RENDER_TRIANGLE_HPP
#define RENDER_TRIANGLE_HPP

#include "Geo/Vector.hpp"
#include "Draw/Framebuffer.hpp"
#include "Draw/Color.hpp"

namespace Render {
	class Triangle
	{
	public:
		struct Vertex
		{
			Geo::Vector position;
			Geo::Vector texCoord;
			Geo::Vector normal;

			Vertex() = default;
			Vertex(const Geo::Vector &_position, const Geo::Vector &_texCoord, const Geo::Vector &_normal) : position(_position), texCoord(_texCoord), normal(_normal) {}
		};

		static void render(Draw::Framebuffer &framebuffer, const Vertex &p0, const Vertex &p1, const Vertex &p2, const Draw::Color &color);
	};
}
#endif