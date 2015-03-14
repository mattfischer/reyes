#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> &&vertices, std::vector<Edge> &&edges, std::vector<Polygon> &&polygons, std::vector<Texture> &&textures)
	: mVertices(std::move(vertices)),
	mEdges(std::move(edges)),
	mPolygons(std::move(polygons)),
	mTextures(std::move(textures))
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

const std::vector<Mesh::Polygon> &Mesh::polygons() const
{
	return mPolygons;
}

const std::vector<Mesh::Texture> &Mesh::textures() const
{
	return mTextures;
}