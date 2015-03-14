#include "Renderer.hpp"

#include "DrawContext.hpp"

#include "Geo/Transformation.hpp"

#include <memory>
#include <algorithm>
#include <climits>

static Geo::Vector clipPlanes[] = {
		{ 1, 0, 0, 1 },
		{ -1, 0, 0, 1 },
		{ 0, 1, 0, 1 },
		{ 0, -1, 0, 1 },
		{ 0, 0, 1, 1 },
		{ 0, 0, -1, 1 }
};

struct ClipPolygon
{
	std::vector<Mesh::Vertex> vertices;
	int numVertices;
};

Renderer::Renderer()
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<Mesh::Edge> edges;
	std::vector<Mesh::Polygon> polygons;

	vertices.push_back(Mesh::Vertex(-1, -1, -1));
	vertices.push_back(Mesh::Vertex(1, -1, -1));
	vertices.push_back(Mesh::Vertex(-1, 1, -1));
	vertices.push_back(Mesh::Vertex(1, 1, -1));
	vertices.push_back(Mesh::Vertex(-1, -1, 1));
	vertices.push_back(Mesh::Vertex(1, -1, 1));
	vertices.push_back(Mesh::Vertex(-1, 1, 1));
	vertices.push_back(Mesh::Vertex(1, 1, 1));

	polygons.push_back(Mesh::Polygon({ 0, 1, 3, 2 }, Color(0xff, 0x00, 0x00)));
	polygons.push_back(Mesh::Polygon({ 4, 5, 7, 6 }, Color(0x00, 0x00, 0xff)));
	polygons.push_back(Mesh::Polygon({ 0, 1, 5, 4 }, Color(0x00, 0xff, 0x00)));
	polygons.push_back(Mesh::Polygon({ 3, 2, 6, 7 }, Color(0xff, 0xff, 0x00)));
	polygons.push_back(Mesh::Polygon({ 0, 2, 6, 4 }, Color(0x00, 0xff, 0xff)));
	polygons.push_back(Mesh::Polygon({ 1, 3, 7, 5 }, Color(0xff, 0x00, 0xff)));

	mMesh = Mesh(std::move(vertices), std::move(edges), std::move(polygons));
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

static bool clipPolygonToPlane(ClipPolygon &polygon, const Geo::Vector &normal)
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

static bool clipPolygon(ClipPolygon &polygon)
{
	for(int i = 0; i < 6; i++) {
		if(!clipPolygonToPlane(polygon, clipPlanes[i])) return false;
	}

	return true;
}

static void renderTriangle(const Geo::Vector &p0, const Geo::Vector &p1, const Geo::Vector &p2, const Color &color, DrawContext &dc)
{
	float x0 = p0.x();
	float y0 = p0.y();
	float z0 = p0.z();
	float x1 = p1.x();
	float y1 = p1.y();
	float z1 = p1.z();
	float x2 = p2.x();
	float y2 = p2.y();
	float z2 = p2.z();

	float det = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
	float winding = (det > 0) ? 1.0f : -1.0f;
	float invdet = 1.0f / det * winding;

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

	float e0 = x21 * ys1 - y21 * xs1;
	float e1 = x02 * ys2 - y02 * xs2;
	float e2 = x10 * ys0 - y10 * xs0;

	float multisampleBiasX[] = { -0.2f, 0.3f, -0.3f, 0.2f };
	float multisampleBiasY[] = { -0.3f, -0.2f, 0.2f, 0.3f };

	for(int y = int(yMin); y <= int(yMax); y++)
	{
		float e0r = e0;
		float e1r = e1;
		float e2r = e2;

		for(int x = int(xMin); x <= int(xMax); x++)
		{
			for(int m = 0; m < 4; m++) {
				float e0m = e0 + multisampleBiasX[m] * -y21 + multisampleBiasY[m] * x21;
				float e1m = e1 + multisampleBiasX[m] * -y02 + multisampleBiasY[m] * x02;
				float e2m = e2 + multisampleBiasX[m] * -y10 + multisampleBiasY[m] * x10;

				if(e0m >= 0 && e1m >= 0 && e2m >= 0) {
					float a = e0m * invdet;
					float b = e1m * invdet;
					float c = e2m * invdet;

					float z = a * z0 + b * z1 + c * z2;
					unsigned short depth = unsigned short(z);
					if(depth <= dc.getDepth(x, y, m)) {
						dc.setPixel(x, y, m, color);
						dc.setDepth(x, y, m, depth);
					}
				}
			}

			e0 -= y21;
			e1 -= y02;
			e2 -= y10;
		}

		e0 = e0r + x21;
		e1 = e1r + x02;
		e2 = e2r + x10;
	}
}

void Renderer::render(Framebuffer &framebuffer)
{
	Geo::Matrix transform = Geo::Transformation::translate(0, 0, 5) * Geo::Transformation::rotate(20, 20, 0);
	Geo::Matrix perspective = Geo::Transformation::perspective(2.0f * float(framebuffer.width()) / float(framebuffer.height()), 2.0f, 1.0f, 10.0f);
	Geo::Matrix viewport = Geo::Transformation::viewport(0.0f, 0.0f, float(framebuffer.width()), float(framebuffer.height()), 0, float(USHRT_MAX));

	std::vector<Mesh::Vertex> vertices = mMesh.vertices();

	for(Mesh::Vertex &vertex : vertices) {
		vertex = transform * vertex;
		vertex = perspective * vertex;
	}

	DrawContext dc(framebuffer);

	dc.fillRect(0, 0, framebuffer.width(), framebuffer.height(), Color(0x80, 0x80, 0x80));
	dc.fillRectDepth(0, 0, framebuffer.width(), framebuffer.height(), USHRT_MAX);
	ClipPolygon clippedPolygon;

	for(const Mesh::Polygon &polygon : mMesh.polygons()) {
		if(clippedPolygon.vertices.size() < polygon.indices.size() + 6) {
			clippedPolygon.vertices.resize(polygon.indices.size() + 6);
		}

		for(unsigned int i = 0; i < polygon.indices.size(); i++) {
			clippedPolygon.vertices[i] = vertices[polygon.indices[i]];
		}
		clippedPolygon.numVertices = polygon.indices.size();

		if(!clipPolygon(clippedPolygon)) {
			continue;
		}

		Geo::Vector p0 = viewport * clippedPolygon.vertices[0].project();
		Geo::Vector p1 = viewport * clippedPolygon.vertices[1].project();
		for(int i = 2; i < clippedPolygon.numVertices; i++) {
			Geo::Vector p2 = viewport * clippedPolygon.vertices[i].project();
			renderTriangle(p0, p1, p2, polygon.color, dc);
			p1 = p2;
		}
	}

	dc.doMultisample();
}