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

static void renderTriangle(const Geo::Vector &p0, const Geo::Vector &p1, const Geo::Vector &p2, DrawContext &dc)
{
	float x0 = p0.x();
	float y0 = p0.y();
	float x1 = p1.x();
	float y1 = p1.y();
	float x2 = p2.x();
	float y2 = p2.y();

	float det = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
	float winding = (det > 0) ? 1.0f : -1.0f;

	float x10 = (x1 - x0) * winding;
	float y10 = (y1 - y0) * winding;
	float x21 = (x2 - x1) * winding;
	float y21 = (y2 - y1) * winding;
	float x02 = (x0 - x2) * winding;
	float y02 = (y0 - y2) * winding;

	float xMin = std::min({ x0, x1, x2 });
	float xMax = std::max({ x0, x1, x2 });
	float yMin = std::min({ y0, y1, y2 });
	float yMax = std::max({ y0, y1, y2 });

	float xs = std::floor(xMin) + 0.5f;
	float ys = std::floor(yMin) + 0.5f;
	float xs0 = xs - x0;
	float ys0 = ys - y0;
	float xs1 = xs - x1;
	float ys1 = ys - y1;
	float xs2 = xs - x2;
	float ys2 = ys - y2;

	float e0 = x10 * ys0 - y10 * xs0;
	float e1 = x21 * ys1 - y21 * xs1;
	float e2 = x02 * ys2 - y02 * xs2;

	for(int y = int(yMin); y <= int(yMax); y++)
	{
		float e0r = e0;
		float e1r = e1;
		float e2r = e2;

		for(int x = int(xMin); x <= int(xMax); x++)
		{
			if(e0 >= 0 && e1 >= 0 && e2 >= 0) {
				dc.setPixel(x, y, DrawContext::Color(0xff, 0xff, 0xff));
			}

			e0 -= y10;
			e1 -= y21;
			e2 -= y02;
		}

		e0 = e0r + x10;
		e1 = e1r + x21;
		e2 = e2r + x02;
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