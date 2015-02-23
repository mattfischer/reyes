#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> &&vertices, std::vector<Edge> &&edges)
	: mVertices(std::move(vertices)),
	mEdges(std::move(edges))
{
}

const std::vector<Mesh::Vertex> &Mesh::vertices()
{
	return mVertices;
}

const std::vector<Mesh::Edge> &Mesh::edges()
{
	return mEdges;
}
