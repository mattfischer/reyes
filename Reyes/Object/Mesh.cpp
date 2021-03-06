#include "Object/Mesh.hpp"
#include "Draw/Context.hpp"
#include "Render/Clipper.hpp"
#include "Render/Rasterize.hpp"

namespace Object {
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

	void Mesh::render(const Render::Config &config) const
	{
		switch(config.type()) {
		case Render::Config::Type::Wireframe:
			renderWireframe(config);
			break;
		case Render::Config::Type::Solid:
			renderSolid(config);
			break;
		}
	}

	void Mesh::renderWireframe(const Render::Config &config) const
	{
		std::vector<Vertex> verts = vertices();

		for(Vertex &vertex : verts) {
			Geo::Vector position = config.projection() * config.view() * transformation() * vertex.position;
			Geo::Vector texCoord = vertex.texCoord;
			Geo::Vector normal = config.view() * transformation() * vertex.normal;
			vertex = Vertex(position, texCoord, normal);
		}

		Draw::Context dc(config.framebuffer());

		for(const Edge &edge : edges()) {
			Geo::Vector a = verts[std::get<0>(edge)].position;
			Geo::Vector b = verts[std::get<1>(edge)].position;

			if(!Render::Clipper::clipLine(a, b)) {
				continue;
			}

			a = config.viewport() * a.project();
			b = config.viewport() * b.project();
			dc.aaline(a.x(), a.y(), b.x(), b.y(), Draw::Color(0.75f, 0.75f, 0.75f));
		}
	}

	void Mesh::renderSolid(const Render::Config &config) const
	{
		std::vector<Vertex> verts = vertices();

		for(Vertex &vertex : verts) {
			Geo::Vector position = config.projection() * config.view() * transformation() * vertex.position;
			Geo::Vector texCoord = vertex.texCoord;
			Geo::Vector normal = config.view() * transformation() * vertex.normal;
			vertex = Vertex(position, texCoord, normal);
		}

		Render::Clipper::Polygon clippedPolygon;
		for(const Polygon &polygon : polygons()) {
			if(clippedPolygon.vertices.size() < polygon.indices.size() + 6) {
				clippedPolygon.vertices.resize(polygon.indices.size() + 6);
			}

			for(unsigned int i = 0; i < polygon.indices.size(); i++) {
				const Vertex &vertex = verts[polygon.indices[i]];
				clippedPolygon.vertices[i] = Render::Clipper::Vertex(vertex.position, vertex.texCoord, vertex.normal);
			}
			clippedPolygon.numVertices = polygon.indices.size();

			if(!Render::Clipper::clipPolygon(clippedPolygon)) {
				continue;
			}

			Geo::Vector p0 = config.viewport() * clippedPolygon.vertices[0].position;
			Geo::Vector p1 = config.viewport() * clippedPolygon.vertices[1].position;
			for(int i = 2; i < clippedPolygon.numVertices; i++) {
				Geo::Vector p2 = config.viewport() * clippedPolygon.vertices[i].position;
				Render::Rasterize::renderQuad(config.framebuffer(), p0, polygon.color, p1, polygon.color, p0, polygon.color, p2, polygon.color, false);
				p1 = p2;
			}
		}
	}
}