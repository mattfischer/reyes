#ifndef MESH_HPP
#define MESH_HPP

#include "Geo/Vector.hpp"

#include <vector>
#include <tuple>

class Mesh
{
public:
	typedef Geo::Vector Vertex;
	typedef std::tuple<int, int> Edge;

	struct Triangle {
		int indices[3];

		Triangle(int i0, int i1, int i2) {
			indices[0] = i0; indices[1] = i1; indices[2] = i2;
		}
	};

	Mesh() = default;
	Mesh(std::vector<Vertex> &&vertices, std::vector<Edge> &&edges, std::vector<Triangle> &&triangles);

	const std::vector<Vertex> &vertices() const;
	const std::vector<Edge> &edges() const;
	const std::vector<Triangle> &triangles() const;

private:
	std::vector<Vertex> mVertices;
	std::vector<Edge> mEdges;
	std::vector<Triangle> mTriangles;
};

#endif