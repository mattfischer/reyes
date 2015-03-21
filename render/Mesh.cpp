#include "Mesh.hpp"
#include "DrawContext.hpp"

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

void Mesh::renderWireframe(Renderer &renderer)
{
	std::vector<Vertex> verts = vertices();

	for(Vertex &vertex : verts) {
		Geo::Vector position = renderer.matrix(Renderer::MatrixType::Projection) * renderer.matrix(Renderer::MatrixType::ModelView) * vertex.position;
		Geo::Vector texCoord = vertex.texCoord;
		Geo::Vector normal = renderer.matrix(Renderer::MatrixType::ModelView) * vertex.normal;
		vertex = Vertex(position, texCoord, normal);
	}

	DrawContext dc(renderer.framebuffer());

	for(const Edge &edge : edges()) {
		Geo::Vector a = verts[std::get<0>(edge)].position;
		Geo::Vector b = verts[std::get<1>(edge)].position;

		if(!renderer.clipLine(a, b)) {
			continue;
		}

		a = renderer.matrix(Renderer::MatrixType::Viewport) * a.project();
		b = renderer.matrix(Renderer::MatrixType::Viewport) * b.project();
		dc.aaline(a.x(), a.y(), b.x(), b.y(), Color(0xc0, 0xc0, 0xc0));
	}
}

void Mesh::renderSolid(Renderer &renderer)
{
	std::vector<Vertex> verts = vertices();

	for(Vertex &vertex : verts) {
		Geo::Vector position = renderer.matrix(Renderer::MatrixType::Projection) * renderer.matrix(Renderer::MatrixType::ModelView) * vertex.position;
		Geo::Vector texCoord = vertex.texCoord;
		Geo::Vector normal = renderer.matrix(Renderer::MatrixType::ModelView) * vertex.normal;
		vertex = Vertex(position, texCoord, normal);
	}

	DrawContext dc(renderer.framebuffer());

	Renderer::Polygon clippedPolygon;
	for(const Polygon &polygon : polygons()) {
		if(clippedPolygon.vertices.size() < polygon.indices.size() + 6) {
			clippedPolygon.vertices.resize(polygon.indices.size() + 6);
		}

		for(unsigned int i = 0; i < polygon.indices.size(); i++) {
			const Vertex &vertex = verts[polygon.indices[i]];
			clippedPolygon.vertices[i] = Renderer::Vertex(vertex.position, vertex.texCoord, vertex.normal);
		}
		clippedPolygon.numVertices = polygon.indices.size();

		if(!renderer.clipPolygon(clippedPolygon)) {
			continue;
		}

		Renderer::Vertex p0(renderer.matrix(Renderer::MatrixType::Viewport) * clippedPolygon.vertices[0].position, clippedPolygon.vertices[0].texCoord, clippedPolygon.vertices[0].normal);
		Renderer::Vertex p1(renderer.matrix(Renderer::MatrixType::Viewport) * clippedPolygon.vertices[1].position, clippedPolygon.vertices[1].texCoord, clippedPolygon.vertices[1].normal);
		for(int i = 2; i < clippedPolygon.numVertices; i++) {
			Renderer::Vertex p2(renderer.matrix(Renderer::MatrixType::Viewport) * clippedPolygon.vertices[i].position, clippedPolygon.vertices[i].texCoord, clippedPolygon.vertices[i].normal);
			renderer.renderTriangle(p0, p1, p2, polygon.color);
			p1 = p2;
		}
	}
}
