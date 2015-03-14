#include "Renderer.hpp"

#include "DrawContext.hpp"

#include "Geo/Transformation.hpp"

#include <memory>
#include <algorithm>

static Geo::Vector clipPlanes[] = {
		{ 1, 0, 0, 1 },
		{ -1, 0, 0, 1 },
		{ 0, 1, 0, 1 },
		{ 0, -1, 0, 1 },
		{ 0, 0, 1, 1 },
		{ 0, 0, -1, 1 }
};

struct Polygon
{
	std::unique_ptr<Mesh::Vertex[]> vertices;
	int numVertices;
};

Renderer::Renderer()
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<Mesh::Edge> edges;
	std::vector<Mesh::Triangle> triangles;

	vertices.push_back(Mesh::Vertex(-1, -1, -1));
	vertices.push_back(Mesh::Vertex(1, -1, -1));
	vertices.push_back(Mesh::Vertex(1, 1, -1));

	triangles.push_back(Mesh::Triangle(0, 1, 2));

	mMesh = Mesh(std::move(vertices), std::move(edges), std::move(triangles));
}

static bool clipLineToPlane(Mesh::Vertex &a, Mesh::Vertex &b, const Geo::Vector &normal)
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

static bool clipPolygonToPlane(Polygon &polygon, const Geo::Vector &normal)
{
	int o = 0;
	Geo::Vector a = polygon.vertices[polygon.numVertices - 1];
	float aN = a * normal;
	Geo::Vector b = polygon.vertices[0];
	for(int i = 0; i < polygon.numVertices; i++) {
		Geo::Vector next = polygon.vertices[i + 1];
		float bN = b * normal;
		if(bN >= 0) {
			if(aN < 0) {
				Geo::Vector m = b - a;
				float mN = m * normal;
				float t = -aN / mN;
				Geo::Vector i = a + t * m;
				polygon.vertices[o] = i;
				o++;
			}
			polygon.vertices[o] = b;
			o++;
		} else {
			if(aN > 0) {
				Geo::Vector m = b - a;
				float mN = m * normal;
				float t = -aN / mN;
				Geo::Vector i = a + t * m;
				polygon.vertices[o] = i;
				o++;
			}
		}
		a = b;
		aN = bN;
		b = next;
	}

	polygon.numVertices = o;
	return polygon.numVertices > 0;
}

static bool clipLine(Mesh::Vertex &a, Mesh::Vertex &b)
{
	for(int i = 0; i < 6; i++) {
		if(!clipLineToPlane(a, b, clipPlanes[i])) return false;
	}

	return true;
}

static bool clipPolygon(Polygon &polygon)
{
	for(int i = 0; i < 6; i++) {
		if(!clipPolygonToPlane(polygon, clipPlanes[i])) return false;
	}

	return true;
}

static float edge(const Geo::Vector &v0, const Geo::Vector &v1, const Geo::Vector &p)
{
	return (v1.x() - v0.x()) * (p.y() - v0.y()) - (v1.y() - v0.y()) * (p.x() - v0.x());
}

static void renderTriangle(const Geo::Vector &p0, const Geo::Vector &p1, const Geo::Vector &p2, DrawContext &dc)
{
	float xMin = std::min({ p0.x(), p1.x(), p2.x() });
	float xMax = std::max({ p0.x(), p1.x(), p2.x() });
	float yMin = std::min({ p0.y(), p1.y(), p2.y() });
	float yMax = std::max({ p0.y(), p1.y(), p2.y() });

	for(int x = int(xMin); x <= int(xMax); x++) {
		for(int y = int(yMin); y <= int(yMax); y++)
		{
			Geo::Vector p(float(x) + 0.5f, float(y) + 0.5f, 0);
			float e0 = edge(p0, p1, p);
			float e1 = edge(p1, p2, p);
			float e2 = edge(p2, p0, p);
			if(e0 >= 0 && e1 >= 0 && e2 >= 0) {
				dc.setPixel(int(x), int(y), DrawContext::Color(0xff, 0xff, 0xff));
			}
		}
	}
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
	Polygon polygon;
	polygon.vertices = std::unique_ptr<Mesh::Vertex[]>(new Mesh::Vertex[3 + 6]);

	for(const Mesh::Triangle &triangle : mMesh.triangles()) {
		for(int i = 0; i < 3; i++) {
			polygon.vertices[i] = vertices[triangle.indices[i]];
		}
		polygon.numVertices = 3;

		if(!clipPolygon(polygon)) {
			continue;
		}

		Geo::Vector p0 = viewport * polygon.vertices[0].project();
		Geo::Vector p1 = viewport * polygon.vertices[1].project();
		for(int i = 2; i < polygon.numVertices; i++) {
			Geo::Vector p2 = viewport * polygon.vertices[i].project();
			renderTriangle(p0, p1, p2, dc);
			p1 = p2;
		}
	}
}