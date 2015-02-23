#include "Renderer.hpp"

#include "DrawContext.hpp"

#include "Geo/Transformation.hpp"

Renderer::Renderer()
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<Mesh::Edge> edges;

	vertices.push_back(Mesh::Vertex(-1, -1, -1));
	vertices.push_back(Mesh::Vertex(1, -1, -1));
	vertices.push_back(Mesh::Vertex(1, 1, -1));
	vertices.push_back(Mesh::Vertex(-1, 1, -1));
	vertices.push_back(Mesh::Vertex(-1, -1, 1));
	vertices.push_back(Mesh::Vertex(1, -1, 1));
	vertices.push_back(Mesh::Vertex(1, 1, 1));
	vertices.push_back(Mesh::Vertex(-1, 1, 1));

	edges.push_back(Mesh::Edge(0, 1));
	edges.push_back(Mesh::Edge(1, 2));
	edges.push_back(Mesh::Edge(2, 3));
	edges.push_back(Mesh::Edge(3, 0));

	edges.push_back(Mesh::Edge(4, 5));
	edges.push_back(Mesh::Edge(5, 6));
	edges.push_back(Mesh::Edge(6, 7));
	edges.push_back(Mesh::Edge(7, 4));

	edges.push_back(Mesh::Edge(0, 4));
	edges.push_back(Mesh::Edge(1, 5));
	edges.push_back(Mesh::Edge(2, 6));
	edges.push_back(Mesh::Edge(3, 7));

	mMesh = Mesh(std::move(vertices), std::move(edges));
}

void Renderer::render(Framebuffer &framebuffer)
{
	Geo::Transformation transform = Geo::Transformation::translate(0, 0, 5);
	Geo::Transformation perspective = Geo::Transformation::perspective(2.0f * float(framebuffer.width()) / float(framebuffer.height()), 2.0f);
	std::vector<Mesh::Vertex> vertices = mMesh.vertices();

	for(Mesh::Vertex &vertex : vertices) {
		vertex = transform * vertex;
		vertex = perspective * vertex;
	}

	DrawContext dc(framebuffer);

	dc.fillRect(0, 0, framebuffer.width(), framebuffer.height(), DrawContext::Color(0x80, 0x80, 0x80));
	for(const Mesh::Edge &edge : mMesh.edges()) {
		const Mesh::Vertex &a = vertices[std::get<0>(edge)];
		const Mesh::Vertex &b = vertices[std::get<1>(edge)];

		float x0 = a.x() / a.w();
		float y0 = a.y() / a.w();
		float x1 = b.x() / b.w();
		float y1 = b.y() / b.w();

		float x0p = (x0 + 1.0f) * framebuffer.width() / 2.0f;
		float y0p = (y0 + 1.0f) * framebuffer.height() / 2.0f;
		float x1p = (x1 + 1.0f) * framebuffer.width() / 2.0f;
		float y1p = (y1 + 1.0f) * framebuffer.height() / 2.0f;

		dc.aaline(x0p, y0p, x1p, y1p, DrawContext::Color(0xff, 0xff, 0xff));
	}
}