#ifndef CLIPPER_HPP
#define CLIPPER_HPP

#include "Geo/Vector.hpp"
#include "Geo/Box.hpp"
#include "Geo/Matrix.hpp"

#include <vector>

namespace Render {
	class Clipper
	{
	public:
		static bool clipLine(Geo::Vector &a, Geo::Vector &b);

		struct Vertex
		{
			Geo::Vector position;
			Geo::Vector texCoord;
			Geo::Vector normal;

			Vertex() = default;
			Vertex(const Geo::Vector &_position, const Geo::Vector &_texCoord, const Geo::Vector &_normal) : position(_position), texCoord(_texCoord), normal(_normal) {}
		};

		struct Polygon
		{
			std::vector<Vertex> vertices;
			int numVertices;
		};

		static bool clipPolygon(Polygon &polygon);

		static bool boxInFrustum(const Geo::Box &box, const Geo::Matrix &transformation);
	};
}

#endif