#ifndef MESH_HPP
#define MESH_HPP

#include "Geo/Point.hpp"

#include <vector>
#include <tuple>

class Mesh
{
public:
	typedef Geo::Point Vertex;
	typedef std::tuple<int, int> Edge;

	Mesh() = default;
	Mesh(std::vector<Vertex> &&vertices, std::vector<Edge> &&edges);

	const std::vector<Vertex> &vertices();
	const std::vector<Edge> &edges();

private:
	std::vector<Vertex> mVertices;
	std::vector<Edge> mEdges;
};

#endif