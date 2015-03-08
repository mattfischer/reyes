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

bool Renderer::clipToPlane(Mesh::Vertex &a, Mesh::Vertex &b, const Geo::Vector &normal)
{
	float aN = a * normal;
	float bN = b * normal;

	if(aN >= 0 && bN >= 0) {
		return true;
	}

	if(aN <= 0 && bN <= 0) {
		return false;
	}

	Geo::Vector m = b - a;
	float mN = m * normal;
	float t = -aN / mN;
	Geo::Vector i = a + t * m;

	if(aN >= 0) {
		b = i;
	} else {
		a = i;
	}

	return true;
}

bool Renderer::clipLine(Mesh::Vertex &a, Mesh::Vertex &b)
{
	if(!clipToPlane(a, b, Geo::Vector(1, 0, 0, 1))) return false;
	if(!clipToPlane(a, b, Geo::Vector(-1, 0, 0, 1))) return false;
	if(!clipToPlane(a, b, Geo::Vector(0, 1, 0, 1))) return false;
	if(!clipToPlane(a, b, Geo::Vector(0, -1, 0, 1))) return false;
	if(!clipToPlane(a, b, Geo::Vector(0, 0, 1, 1))) return false;
	if(!clipToPlane(a, b, Geo::Vector(0, 0, -1, 1))) return false;

	return true;
}

void Renderer::render(Framebuffer &framebuffer)
{
	Geo::Matrix transform = Geo::Transformation::translate(0, 0, 5);
	Geo::Matrix perspective = Geo::Transformation::perspective(2.0f * float(framebuffer.width()) / float(framebuffer.height()), 2.0f, 1.0f, 10.0f);
	Geo::Matrix viewport = Geo::Transformation::viewport(0.0f, 0.0f, float(framebuffer.width()), float(framebuffer.height()));

	std::vector<Mesh::Vertex> vertices = mMesh.vertices();

	for(Mesh::Vertex &vertex : vertices) {
		vertex = transform * vertex;
		vertex = perspective * vertex;
	}

	DrawContext dc(framebuffer);

	dc.fillRect(0, 0, framebuffer.width(), framebuffer.height(), DrawContext::Color(0x80, 0x80, 0x80));
	for(const Mesh::Edge &edge : mMesh.edges()) {
		Mesh::Vertex a = vertices[std::get<0>(edge)];
		Mesh::Vertex b = vertices[std::get<1>(edge)];

		if(!clipLine(a, b)) {
			continue;
		}

		a = viewport * a.project();
		b = viewport * b.project();

		dc.aaline(a.x(), a.y(), b.x(), b.y(), DrawContext::Color(0xff, 0xff, 0xff));
	}
}