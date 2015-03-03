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

bool Renderer::clipToPlane(Mesh::Vertex &a, Mesh::Vertex &b, const Geo::Coordinate &normal)
{
	Geo::Coordinate m(b.x() - a.x(), b.y() - a.y(), b.z() - a.z(), b.w() - a.w());

	// p = a + m * t
	// p * normal > 0
	// a * normal + m * normal * t > 0

	float aN = normal.x() * a.x() + normal.y() * a.y() + normal.z() * a.z() + normal.w() * a.w();
	float mN = normal.x() * m.x() + normal.y() * m.y() + normal.z() * m.z() + normal.w() * m.w();

	// mN * t > -aN

	if(mN >= 0) {
		if(-aN <= 0) {
			return true;
		}
		else if(-aN >= mN) {
			return false;
		}
		else {
			float t = -aN / mN;
			a = Geo::Point(a.x() + t * m.x(), a.y() + t * m.y(), a.z() + t * m.z(), a.w() + t * m.w());
			return true;
		}
	}
	else {
		if(-aN >= 0) {
			return false;
		}
		else if(-aN <= mN) {
			return true;
		}
		else {
			float t = -aN / mN;
			b = Geo::Point(a.x() + t * m.x(), a.y() + t * m.y(), a.z() + t * m.z(), a.w() + t * m.w());
			return true;
		}
	}
}

bool Renderer::clipLine(Mesh::Vertex &a, Mesh::Vertex &b)
{
	Geo::Coordinate normal;

	if(!clipToPlane(a, b, Geo::Coordinate(1, 0, 0, 1))) return false;
	if(!clipToPlane(a, b, Geo::Coordinate(-1, 0, 0, 1))) return false;
	if(!clipToPlane(a, b, Geo::Coordinate(0, 1, 0, 1))) return false;
	if(!clipToPlane(a, b, Geo::Coordinate(0, -1, 0, 1))) return false;
	//if(!clipToPlane(a, b, Geo::Coordinate(0, 0, 1, 1))) return false;
	//if(!clipToPlane(a, b, Geo::Coordinate(0, 0, -1, 1))) return false;

	return true;
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
		Mesh::Vertex a = vertices[std::get<0>(edge)];
		Mesh::Vertex b = vertices[std::get<1>(edge)];

		if(!clipLine(a, b)) {
			continue;
		}

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