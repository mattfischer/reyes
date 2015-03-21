#include "Patch.hpp"

#include "DrawContext.hpp"

Patch::Patch(Geo::Vector points[16])
{
	mPoints = std::make_unique<Geo::Vector[]>(16);

	for(int i = 0; i < 16; i++) {
		mPoints[i] = points[i];
	}
}

Patch::Patch(Patch &&other)
	: mPoints(std::move(other.mPoints))
{
}

const Geo::Vector &Patch::point(int x, int y) const
{
	return mPoints[y * 4 + x];
}

Grid Patch::tesselate(int divisions) const
{
	Grid grid(divisions + 1, divisions + 1);

	for(int i = 0; i <= divisions; i++) {
		for(int j = 0; j <= divisions; j++) {
			float s = float(i) / float(divisions);
			float t = float(j) / float(divisions);

			Geo::Vector interp[9];
			for(int k = 0; k < 3; k++) {
				for(int l = 0; l < 3; l++) {
					interp[k * 3 + l] = point(l, k) * s * t + point(l + 1, k) * (1 - s) * t + point(l, k + 1) * s * (1 - t) + point(l + 1, k + 1) * (1 - s) * (1 - t);
				}
			}

			for(int k = 0; k < 2; k++) {
				for(int l = 0; l < 2; l++) {
					interp[k * 3 + l] = interp[k * 3 + l] * s * t + interp[k * 3 + l + 1] * (1 - s) * t + interp[(k + 1) * 3 + l] * s * (1 - t) + interp[(k + 1) * 3 + l + 1] * (1 - s) * (1 - t);
				}
			}

			Geo::Vector point = interp[0 * 3 + 0] * s * t + interp[0 * 3 + 1] * (1 - s) * t + interp[1 * 3 + 0] * s * (1 - t) + interp[1 * 3 + 1] * (1 - s) * (1 - t);
			Geo::Vector u = (interp[1 * 3 + 0] - interp[0 * 3 + 0]) * s + (interp[1 * 3 + 1] - interp[0 * 3 + 1]) * (1 - s);
			Geo::Vector v = (interp[0 * 3 + 1] - interp[0 * 3 + 0]) * t + (interp[1 * 3 + 1] - interp[1 * 3 + 0]) * (1 - t);
			Geo::Vector normal = v % u;
			normal.setW(0);
			grid.setPoint(i, j, Grid::Point(point, Geo::Vector(s, t, 0), normal.normalize()));
		}
	}

	return grid;
}

void Patch::renderWireframe(Renderer &renderer)
{
	DrawContext dc(renderer.framebuffer());

	Grid grid = tesselate(16);

	for(int x = 0; x < grid.width(); x++) {
		for(int y = 0; y < grid.height(); y++) {
			Geo::Vector position = renderer.matrix(Renderer::MatrixType::Projection) * renderer.matrix(Renderer::MatrixType::ModelView) * grid.point(x, y).position;
			Geo::Vector texCoord = grid.point(x, y).texCoord;
			Geo::Vector normal = renderer.matrix(Renderer::MatrixType::ModelView) * grid.point(x, y).normal;
			grid.setPoint(x, y, Grid::Point(position, texCoord, normal));
		}
	}

	for(int x = 0; x < grid.width(); x++) {
		for(int y = 0; y < grid.height(); y++) {
			if(x > 0) {
				Geo::Vector a = grid.point(x - 1, y).position;
				Geo::Vector b = grid.point(x, y).position;

				if(!renderer.clipLine(a, b)) {
					continue;
				}

				a = renderer.matrix(Renderer::MatrixType::Viewport) * a.project();
				b = renderer.matrix(Renderer::MatrixType::Viewport) * b.project();
				dc.aaline(a.x(), a.y(), b.x(), b.y(), Color(0xc0, 0xc0, 0xc0));
			}

			if(y > 0) {
				Geo::Vector a = grid.point(x, y - 1).position;
				Geo::Vector b = grid.point(x, y).position;

				if(!renderer.clipLine(a, b)) {
					continue;
				}

				a = renderer.matrix(Renderer::MatrixType::Viewport) * a.project();
				b = renderer.matrix(Renderer::MatrixType::Viewport) * b.project();
				dc.aaline(a.x(), a.y(), b.x(), b.y(), Color(0xc0, 0xc0, 0xc0));
			}
		}
	}
}

void Patch::renderSolid(Renderer &renderer)
{
	DrawContext dc(renderer.framebuffer());

	Grid grid = tesselate(16);

	for(int x = 0; x < grid.width(); x++) {
		for(int y = 0; y < grid.height(); y++) {
			Geo::Vector position = renderer.matrix(Renderer::MatrixType::Projection) * renderer.matrix(Renderer::MatrixType::ModelView) * grid.point(x, y).position;
			Geo::Vector texCoord = grid.point(x, y).texCoord;
			Geo::Vector normal = renderer.matrix(Renderer::MatrixType::ModelView) * grid.point(x, y).normal;
			grid.setPoint(x, y, Grid::Point(position, texCoord, normal));
		}
	}

	Renderer::Polygon clippedPolygon;
	clippedPolygon.vertices.resize(4 + 6);
	for(int x = 0; x < grid.width() - 1; x++) {
		for(int y = 0; y < grid.height() - 1; y++) {
			clippedPolygon.numVertices = 4;
			clippedPolygon.vertices[0] = Renderer::Vertex(grid.point(x, y).position, grid.point(x, y).texCoord, grid.point(x, y).normal);
			clippedPolygon.vertices[1] = Renderer::Vertex(grid.point(x + 1, y).position, grid.point(x + 1, y).texCoord, grid.point(x + 1, y).normal);
			clippedPolygon.vertices[2] = Renderer::Vertex(grid.point(x + 1, y + 1).position, grid.point(x + 1, y + 1).texCoord, grid.point(x + 1, y + 1).normal);
			clippedPolygon.vertices[3] = Renderer::Vertex(grid.point(x, y + 1).position, grid.point(x, y + 1).texCoord, grid.point(x, y + 1).normal);

			if(!renderer.clipPolygon(clippedPolygon)) {
				continue;
			}

			Renderer::Vertex p0(renderer.matrix(Renderer::MatrixType::Viewport) * clippedPolygon.vertices[0].position, clippedPolygon.vertices[0].texCoord, clippedPolygon.vertices[0].normal);
			Renderer::Vertex p1(renderer.matrix(Renderer::MatrixType::Viewport) * clippedPolygon.vertices[1].position, clippedPolygon.vertices[1].texCoord, clippedPolygon.vertices[1].normal);
			for(int i = 2; i < clippedPolygon.numVertices; i++) {
				Renderer::Vertex p2(renderer.matrix(Renderer::MatrixType::Viewport) * clippedPolygon.vertices[i].position, clippedPolygon.vertices[i].texCoord, clippedPolygon.vertices[i].normal);
				renderer.renderTriangle(p0, p1, p2, Color(0xff, 0x0, 0x0));
				p1 = p2;
			}
		}
	}
}
