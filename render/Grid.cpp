#include "Grid.hpp"

#include "DrawContext.hpp"

Grid::Grid(int width, int height)
{
	mWidth = width;
	mHeight = height;
	mPoints = std::make_unique<Point[]>(width * height);
}

Grid::Grid(Grid &&other)
	: mPoints(std::move(other.mPoints))
{
	mWidth = other.mWidth;
	mHeight = other.mHeight;
}

int Grid::width() const
{
	return mWidth;
}

int Grid::height() const
{
	return mHeight;
}

void Grid::setPoint(int x, int y, const Point &point)
{
	mPoints[y * mWidth + x] = point;
}

const Grid::Point &Grid::point(int x, int y) const
{
	return mPoints[y * mWidth + x];
}

void Grid::renderWireframe(Renderer &renderer)
{
	DrawContext dc(renderer.framebuffer());

	for(int x = 0; x < width(); x++) {
		for(int y = 0; y < height(); y++) {
			if(x > 0) {
				Geo::Vector a = point(x - 1, y).position;
				Geo::Vector b = point(x, y).position;

				if(!renderer.clipLine(a, b)) {
					continue;
				}

				a = renderer.matrix(Renderer::MatrixType::Viewport) * a.project();
				b = renderer.matrix(Renderer::MatrixType::Viewport) * b.project();
				dc.aaline(a.x(), a.y(), b.x(), b.y(), Color(0xc0, 0xc0, 0xc0));
			}

			if(y > 0) {
				Geo::Vector a = point(x, y - 1).position;
				Geo::Vector b = point(x, y).position;

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

void Grid::renderSolid(Renderer &renderer)
{
	DrawContext dc(renderer.framebuffer());

	Renderer::Polygon clippedPolygon;
	clippedPolygon.vertices.resize(4 + 6);
	for(int x = 0; x < width() - 1; x++) {
		for(int y = 0; y < height() - 1; y++) {
			clippedPolygon.numVertices = 4;
			clippedPolygon.vertices[0] = Renderer::Vertex(point(x, y).position, point(x, y).texCoord, point(x, y).normal);
			clippedPolygon.vertices[1] = Renderer::Vertex(point(x + 1, y).position, point(x + 1, y).texCoord, point(x + 1, y).normal);
			clippedPolygon.vertices[2] = Renderer::Vertex(point(x + 1, y + 1).position, point(x + 1, y + 1).texCoord, point(x + 1, y + 1).normal);
			clippedPolygon.vertices[3] = Renderer::Vertex(point(x, y + 1).position, point(x, y + 1).texCoord, point(x, y + 1).normal);

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
