#ifndef CLIPPER_HPP
#define CLIPPER_HPP

#include "Geo/Vector.hpp"

#include <vector>

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

	static bool Clipper::clipPolygon(Polygon &polygon);
};

#endif