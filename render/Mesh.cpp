#include "Mesh.hpp"
#include "DrawContext.hpp"
#include "Clipper.hpp"
#include "Triangle.hpp"

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

void Mesh::render(const RenderConfig &config) const
{
	switch(config.type()) {
	case RenderConfig::Type::Wireframe:
		renderWireframe(config);
		break;
	case RenderConfig::Type::Solid:
		renderSolid(config);
		break;
	}
}

void Mesh::renderWireframe(const RenderConfig &config) const
{
	std::vector<Vertex> verts = vertices();

	for(Vertex &vertex : verts) {
		Geo::Vector position = config.projection() * config.view() * transformation() * vertex.position;
		Geo::Vector texCoord = vertex.texCoord;
		Geo::Vector normal = config.view() * transformation() * vertex.normal;
		vertex = Vertex(position, texCoord, normal);
	}

	DrawContext dc(config.framebuffer());

	for(const Edge &edge : edges()) {
		Geo::Vector a = verts[std::get<0>(edge)].position;
		Geo::Vector b = verts[std::get<1>(edge)].position;

		if(!Clipper::clipLine(a, b)) {
			continue;
		}

		a = config.viewport() * a.project();
		b = config.viewport() * b.project();
		dc.aaline(a.x(), a.y(), b.x(), b.y(), Color(0xc0, 0xc0, 0xc0));
	}
}

void Mesh::renderSolid(const RenderConfig &config) const
{
	std::vector<Vertex> verts = vertices();

	for(Vertex &vertex : verts) {
		Geo::Vector position = config.projection() * config.view() * transformation() * vertex.position;
		Geo::Vector texCoord = vertex.texCoord;
		Geo::Vector normal = config.view() * transformation() * vertex.normal;
		vertex = Vertex(position, texCoord, normal);
	}

	DrawContext dc(config.framebuffer());

	Clipper::Polygon clippedPolygon;
	for(const Polygon &polygon : polygons()) {
		if(clippedPolygon.vertices.size() < polygon.indices.size() + 6) {
			clippedPolygon.vertices.resize(polygon.indices.size() + 6);
		}

		for(unsigned int i = 0; i < polygon.indices.size(); i++) {
			const Vertex &vertex = verts[polygon.indices[i]];
			clippedPolygon.vertices[i] = Clipper::Vertex(vertex.position, vertex.texCoord, vertex.normal);
		}
		clippedPolygon.numVertices = polygon.indices.size();

		if(!Clipper::clipPolygon(clippedPolygon)) {
			continue;
		}

		Triangle::Vertex p0(config.viewport() * clippedPolygon.vertices[0].position, clippedPolygon.vertices[0].texCoord, clippedPolygon.vertices[0].normal);
		Triangle::Vertex p1(config.viewport() * clippedPolygon.vertices[1].position, clippedPolygon.vertices[1].texCoord, clippedPolygon.vertices[1].normal);
		for(int i = 2; i < clippedPolygon.numVertices; i++) {
			Triangle::Vertex p2(config.viewport() * clippedPolygon.vertices[i].position, clippedPolygon.vertices[i].texCoord, clippedPolygon.vertices[i].normal);
			Triangle::render(config.framebuffer(), p0, p1, p2, polygon.color);
			p1 = p2;
		}
	}
}
