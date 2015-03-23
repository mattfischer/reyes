#include "Render/Grid.hpp"

#include "Render/Clipper.hpp"
#include "Render/Triangle.hpp"
#include "Draw/Context.hpp"

namespace Render {
	Grid::Grid(int width, int height, const Draw::Color &color)
		: mColor(color)
	{
		mWidth = width;
		mHeight = height;
		mPoints = std::make_unique<Point[]>(width * height);
	}

	Grid::Grid(Grid &&other)
		: mPoints(std::move(other.mPoints)), mColor(other.mColor)
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

	void Grid::render(const Config &config) const
	{
		switch(config.type()) {
		case Config::Type::Wireframe:
			renderWireframe(config);
			break;
		case Config::Type::Solid:
			renderSolid(config);
			break;
		}
	}

	void Grid::renderWireframe(const Config &config) const
	{
		Draw::Context dc(config.framebuffer());

		for(int x = 0; x < width(); x++) {
			for(int y = 0; y < height(); y++) {
				if(x > 0) {
					Geo::Vector a = point(x - 1, y).position;
					Geo::Vector b = point(x, y).position;

					if(!Clipper::clipLine(a, b)) {
						continue;
					}

					a = config.viewport() * a.project();
					b = config.viewport() * b.project();
					dc.aaline(a.x(), a.y(), b.x(), b.y(), Draw::Color(0xc0, 0xc0, 0xc0));
				}

				if(y > 0) {
					Geo::Vector a = point(x, y - 1).position;
					Geo::Vector b = point(x, y).position;

					if(!Clipper::clipLine(a, b)) {
						continue;
					}

					a = config.viewport() * a.project();
					b = config.viewport() * b.project();
					dc.aaline(a.x(), a.y(), b.x(), b.y(), Draw::Color(0xc0, 0xc0, 0xc0));
				}
			}
		}
	}

	void Grid::renderSolid(const Config &config) const
	{
		Clipper::Polygon clippedPolygon;
		clippedPolygon.vertices.resize(4 + 6);
		for(int x = 0; x < width() - 1; x++) {
			for(int y = 0; y < height() - 1; y++) {
				clippedPolygon.numVertices = 4;
				clippedPolygon.vertices[0] = Clipper::Vertex(point(x, y).position, point(x, y).texCoord, point(x, y).normal);
				clippedPolygon.vertices[1] = Clipper::Vertex(point(x + 1, y).position, point(x + 1, y).texCoord, point(x + 1, y).normal);
				clippedPolygon.vertices[2] = Clipper::Vertex(point(x + 1, y + 1).position, point(x + 1, y + 1).texCoord, point(x + 1, y + 1).normal);
				clippedPolygon.vertices[3] = Clipper::Vertex(point(x, y + 1).position, point(x, y + 1).texCoord, point(x, y + 1).normal);

				if(!Clipper::clipPolygon(clippedPolygon)) {
					continue;
				}

				Triangle::Vertex p0(config.viewport() * clippedPolygon.vertices[0].position, clippedPolygon.vertices[0].texCoord, clippedPolygon.vertices[0].normal);
				Triangle::Vertex p1(config.viewport() * clippedPolygon.vertices[1].position, clippedPolygon.vertices[1].texCoord, clippedPolygon.vertices[1].normal);
				for(int i = 2; i < clippedPolygon.numVertices; i++) {
					Triangle::Vertex p2(config.viewport() * clippedPolygon.vertices[i].position, clippedPolygon.vertices[i].texCoord, clippedPolygon.vertices[i].normal);
					Triangle::render(config.framebuffer(), p0, p1, p2, mColor);
					p1 = p2;
				}
			}
		}
	}
}