#include "Renderer.hpp"

#include "DrawContext.hpp"

#include "Geo/Transformation.hpp"

#include "PatchSet.hpp"
#include "BptFileLoader.hpp"

#include <memory>
#include <algorithm>
#include <climits>
#include <string>
#include <fstream>
#include <algorithm>

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

Renderer::Renderer(Framebuffer &framebuffer)
	: mFramebuffer(framebuffer)
{
	setMatrix(MatrixType::ModelView, Geo::Transformation::translate(0, -2, 5) * Geo::Transformation::rotate(-100, 0, 0));
	setMatrix(MatrixType::Projection, Geo::Transformation::perspective(2.0f * float(mFramebuffer.width()) / float(mFramebuffer.height()), 2.0f, 1.0f, 10.0f));
	setMatrix(MatrixType::Viewport, Geo::Transformation::viewport(0.0f, 0.0f, float(mFramebuffer.width()), float(mFramebuffer.height()), 0, float(USHRT_MAX)));

	std::vector<Mesh::Vertex> vertices;
	std::vector<Mesh::Edge> edges;
	std::vector<Mesh::Polygon> polygons;
	std::vector<Mesh::Texture> textures;

	mPatchSet = BptFileLoader::load("teapot.bpt");
}

void Renderer::setMatrix(MatrixType type, const Geo::Matrix &matrix)
{
	mMatrices[unsigned int(type)] = matrix;
}

const Geo::Matrix &Renderer::matrix(MatrixType type)
{
	return mMatrices[unsigned int(type)];
}

static bool clipLineToPlane(Geo::Vector &a, Geo::Vector &b, const Geo::Vector &normal)
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
	Mesh::Vertex a = polygon.vertices[polygon.numVertices - 1];
	float aN = a.position * normal;
	Mesh::Vertex b = polygon.vertices[0];
	for(int i = 0; i < polygon.numVertices; i++) {
		Mesh::Vertex next = polygon.vertices[i + 1];
		float bN = b.position * normal;
		if(bN >= 0) {
			if(aN < 0) {
				Geo::Vector m = b.position - a.position;
				Geo::Vector mt = b.texCoord - a.texCoord;
				Geo::Vector mn = b.normal - a.normal;
				float mN = m * normal;
				float t = -aN / mN;
				Geo::Vector i = a.position + t * m;
				Geo::Vector it = a.texCoord + t * mt;
				Geo::Vector in = a.normal + t * mn;
				polygon.vertices[o] = Mesh::Vertex(i, it, in);
				o++;
			}
			polygon.vertices[o] = b;
			o++;
		} else {
			if(aN > 0) {
				Geo::Vector m = b.position - a.position;
				Geo::Vector mt = b.texCoord - a.texCoord;
				Geo::Vector mn = b.normal - a.normal;
				float mN = m * normal;
				float t = -aN / mN;
				Geo::Vector i = a.position + t * m;
				Geo::Vector it = a.texCoord + t * mt;
				Geo::Vector in = a.normal + t * mn;
				polygon.vertices[o] = Mesh::Vertex(i, it, in);
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

static bool clipLine(Geo::Vector &a, Geo::Vector &b)
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

static void renderTriangle(const Mesh::Vertex &p0, const Mesh::Vertex &p1, const Mesh::Vertex &p2, const Color &color, DrawContext &dc)
{
	Geo::Vector pv0 = p0.position.project();
	Geo::Vector pv1 = p1.position.project();
	Geo::Vector pv2 = p2.position.project();

	float x0 = pv0.x();
	float y0 = pv0.y();
	float z0 = pv0.z();
	float w0 = p0.position.w();
	float x1 = pv1.x();
	float y1 = pv1.y();
	float z1 = pv1.z();
	float w1 = p1.position.w();
	float x2 = pv2.x();
	float y2 = pv2.y();
	float z2 = pv2.z();
	float w2 = p2.position.w();

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

	float X0 = -y21;
	float X1 = -y02;
	float X2 = -y10;

	float Y0 = x21;
	float Y1 = x02;
	float Y2 = x10;

	float e0 = x21 * ys1 - y21 * xs1;
	float e1 = x02 * ys2 - y02 * xs2;
	float e2 = x10 * ys0 - y10 * xs0;

	float iw0 = 1.0f / w0;
	float iw1 = 1.0f / w1;
	float iw2 = 1.0f / w2;

	Geo::Vector texCoord0 = p0.texCoord * iw0;
	Geo::Vector texCoord1 = p1.texCoord * iw1;
	Geo::Vector texCoord2 = p2.texCoord * iw2;

	Geo::Vector normal0 = p0.normal * iw0;
	Geo::Vector normal1 = p1.normal * iw1;
	Geo::Vector normal2 = p2.normal * iw2;

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
				float e0m = e0 + X0 * multisampleBiasX[m] + Y0 * multisampleBiasY[m];
				float e1m = e1 + X1 * multisampleBiasX[m] + Y1 * multisampleBiasY[m];
				float e2m = e2 + X2 * multisampleBiasX[m] + Y2 * multisampleBiasY[m];

				if(e0m >= 0 && e1m >= 0 && e2m >= 0) {
					float a = e0m * invdet;
					float b = e1m * invdet;
					float c = e2m * invdet;

					float z = a * z0 + b * z1 + c * z2;
					unsigned short depth = unsigned short(z);
					if(depth <= dc.getDepth(x, y, m)) {
						float iw = a * iw0 + b * iw1 + c * iw2;
						float w = 1.0f / iw;
						Geo::Vector texCoord = (a * texCoord0 + b * texCoord1 + c * texCoord2) * w;
						Geo::Vector normal = (a * normal0 + b * normal1 + c * normal2) * w;

						float s = texCoord.x();
						float t = texCoord.y();
						int si = int(std::floor(s));
						int ti = int(std::floor(t));
						float sf = s - float(si);
						float tf = t - float(ti);
						/*
						Color c0 = texture.data[ti * texture.width + si];
						Color cs = texture.data[ti * texture.width + si + 1];
						Color ct = texture.data[(ti + 1) * texture.width + si];
						Color cst = texture.data[(ti + 1) * texture.width + si + 1];
						Color c = c0 * sf * tf + cs * (1 - s) * t + ct * s * (1 - t) + cst * (1 - s) * (1 - t);
						*/
						float l = std::max(normal * (Geo::Vector(1, 1, -1, 0).normalize()), 0.0f);
						dc.setPixel(x, y, m, color * l);
						dc.setDepth(x, y, m, depth);
					}
				}
			}

			e0 += X0;
			e1 += X1;
			e2 += X2;
		}

		e0 = e0r + Y0;
		e1 = e1r + Y1;
		e2 = e2r + Y2;
	}
}

void Renderer::renderMeshWireframe(const Mesh &mesh)
{
	std::vector<Mesh::Vertex> vertices = mesh.vertices();

	for(Mesh::Vertex &vertex : vertices) {
		vertex = Mesh::Vertex(matrix(MatrixType::ModelView) * vertex.position, vertex.texCoord, matrix(MatrixType::ModelView) * vertex.normal);
		vertex = Mesh::Vertex(matrix(MatrixType::Projection) * vertex.position, vertex.texCoord, vertex.normal);
	}

	DrawContext dc(mFramebuffer);

	for(const Mesh::Edge &edge : mesh.edges()) {
		Geo::Vector a = vertices[std::get<0>(edge)].position;
		Geo::Vector b = vertices[std::get<1>(edge)].position;

		if(!clipLine(a, b)) {
			continue;
		}

		a = matrix(MatrixType::Viewport) * a.project();
		b = matrix(MatrixType::Viewport) * b.project();
		dc.aaline(a.x(), a.y(), b.x(), b.y(), Color(0xc0, 0xc0, 0xc0));
	}
}

void Renderer::renderMeshPolygons(const Mesh &mesh)
{
	std::vector<Mesh::Vertex> vertices = mesh.vertices();

	for(Mesh::Vertex &vertex : vertices) {
		vertex = Mesh::Vertex(matrix(MatrixType::ModelView) * vertex.position, vertex.texCoord, matrix(MatrixType::ModelView) * vertex.normal);
		vertex = Mesh::Vertex(matrix(MatrixType::Projection) * vertex.position, vertex.texCoord, vertex.normal);
	}

	DrawContext dc(mFramebuffer);

	ClipPolygon clippedPolygon;
	for(const Mesh::Polygon &polygon : mesh.polygons()) {
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

		Mesh::Vertex p0(matrix(MatrixType::Viewport) * clippedPolygon.vertices[0].position, clippedPolygon.vertices[0].texCoord, clippedPolygon.vertices[0].normal);
		Mesh::Vertex p1(matrix(MatrixType::Viewport) * clippedPolygon.vertices[1].position, clippedPolygon.vertices[1].texCoord, clippedPolygon.vertices[1].normal);
		for(int i = 2; i < clippedPolygon.numVertices; i++) {
			Mesh::Vertex p2(matrix(MatrixType::Viewport) * clippedPolygon.vertices[i].position, clippedPolygon.vertices[i].texCoord, clippedPolygon.vertices[i].normal);
			renderTriangle(p0, p1, p2, polygon.color, dc);
			p1 = p2;
		}
	}
}

void Renderer::renderPatchSetPolygons(const PatchSet &patchSet)
{
	DrawContext dc(mFramebuffer);

	for(const Patch &patch : patchSet) {
		Grid grid = patch.tesselate(16);

		ClipPolygon clippedPolygon;
		clippedPolygon.vertices.resize(4 + 6);
		for(int x = 0; x < grid.width() - 1; x++) {
			for(int y = 0; y < grid.height() - 1; y++) {
				clippedPolygon.numVertices = 4;
				clippedPolygon.vertices[0] = Mesh::Vertex(grid.point(x, y).position, Geo::Vector(), grid.point(x, y).normal);
				clippedPolygon.vertices[1] = Mesh::Vertex(grid.point(x + 1, y).position, Geo::Vector(), grid.point(x + 1, y).normal);
				clippedPolygon.vertices[2] = Mesh::Vertex(grid.point(x + 1, y + 1).position, Geo::Vector(), grid.point(x + 1, y + 1).normal);
				clippedPolygon.vertices[3] = Mesh::Vertex(grid.point(x, y + 1).position, Geo::Vector(), grid.point(x, y + 1).normal);

				for(int i=0; i<4; i++) {
					clippedPolygon.vertices[i] = Mesh::Vertex(matrix(MatrixType::Projection) * matrix(MatrixType::ModelView) * clippedPolygon.vertices[i].position, clippedPolygon.vertices[i].texCoord, matrix(MatrixType::ModelView) * clippedPolygon.vertices[i].normal);
				}

				if(!clipPolygon(clippedPolygon)) {
					continue;
				}

				Mesh::Vertex p0(matrix(MatrixType::Viewport) * clippedPolygon.vertices[0].position, clippedPolygon.vertices[0].texCoord, clippedPolygon.vertices[0].normal);
				Mesh::Vertex p1(matrix(MatrixType::Viewport) * clippedPolygon.vertices[1].position, clippedPolygon.vertices[1].texCoord, clippedPolygon.vertices[1].normal);
				for(int i = 2; i < clippedPolygon.numVertices; i++) {
					Mesh::Vertex p2(matrix(MatrixType::Viewport) * clippedPolygon.vertices[i].position, clippedPolygon.vertices[i].texCoord, clippedPolygon.vertices[i].normal);
					renderTriangle(p0, p1, p2, Color(0xff, 0x0, 0x0), dc);
					p1 = p2;
				}
			}
		}
	}
}

void Renderer::renderPatchSetWireframe(const PatchSet &patchSet)
{
	DrawContext dc(mFramebuffer);

	for(const Patch &patch : patchSet) {
		Grid grid = patch.tesselate(16);

		for(int x = 0; x < grid.width(); x++) {
			for(int y = 0; y < grid.height(); y++) {
				if(x > 0) {
					Geo::Vector a = matrix(MatrixType::Projection) * matrix(MatrixType::ModelView) * grid.point(x - 1, y).position;
					Geo::Vector b = matrix(MatrixType::Projection) * matrix(MatrixType::ModelView) * grid.point(x, y).position;

					if(!clipLine(a, b)) {
						continue;
					}

					a = matrix(MatrixType::Viewport) * a.project();
					b = matrix(MatrixType::Viewport) * b.project();
					dc.aaline(a.x(), a.y(), b.x(), b.y(), Color(0xc0, 0xc0, 0xc0));
				}

				if(y > 0) {
					Geo::Vector a = matrix(MatrixType::Projection) * matrix(MatrixType::ModelView) * grid.point(x, y - 1).position;
					Geo::Vector b = matrix(MatrixType::Projection) * matrix(MatrixType::ModelView) * grid.point(x, y).position;

					if(!clipLine(a, b)) {
						continue;
					}

					a = matrix(MatrixType::Viewport) * a.project();
					b = matrix(MatrixType::Viewport) * b.project();
					dc.aaline(a.x(), a.y(), b.x(), b.y(), Color(0xc0, 0xc0, 0xc0));
				}
			}
		}
	}
}

void Renderer::render()
{
	DrawContext dc(mFramebuffer);

	dc.fillRect(0, 0, mFramebuffer.width(), mFramebuffer.height(), Color(0x80, 0x80, 0x80));
	dc.fillRectDepth(0, 0, mFramebuffer.width(), mFramebuffer.height(), USHRT_MAX);

	renderPatchSetPolygons(mPatchSet);

	dc.doMultisample();

	//renderPatchSetWireframe(mPatchSet);
}