#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> &&vertices, std::vector<Edge> &&edges, std::vector<Triangle> &&triangles)
	: mVertices(std::move(vertices)),
	mEdges(std::move(edges)),
	mTriangles(std::move(triangles))
{
}

const std::vector<Mesh::Vertex> &Mesh::vertices() const
{
	return mVertices;
}

const std::vector<Mesh::Edge> &Mesh::edges() const
{
	return mEdges;
}

const std::vector<Mesh::Triangle> &Mesh::triangles() const
{
	return mTriangles;
}
