#ifndef MESH_HPP
#define MESH_HPP

#include "Geo/Vector.hpp"
#include "Color.hpp"

#include <vector>
#include <memory>
#include <tuple>

class Mesh
{
public:
	typedef Geo::Vector Vertex;
	typedef std::tuple<int, int> Edge;

	struct Polygon {
		std::vector<int> indices;
		Color color;

		Polygon(std::initializer_list<int> _indices, Color &_color) : indices(_indices), color(_color) {}
	};

	Mesh() = default;
	Mesh(std::vector<Vertex> &&vertices, std::vector<Edge> &&edges, std::vector<Polygon> &&polygons);

	const std::vector<Vertex> &vertices() const;
	const std::vector<Edge> &edges() const;
	const std::vector<Polygon> &polygons() const;

private:
	std::vector<Vertex> mVertices;
	std::vector<Edge> mEdges;
	std::vector<Polygon> mPolygons;
};

#endif