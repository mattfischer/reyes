#include "Render/Grid.hpp"

#include "Render/Clipper.hpp"
#include "Render/Rasterize.hpp"
#include "Draw/Context.hpp"

namespace Render {
	Grid::Grid(int width, int height)
	{
		mWidth = width;
		mHeight = height;
		mPoints = std::make_unique<Geo::Vector[]>((width + 1) * (height + 1));
		mColors = std::make_unique<Draw::Color[]>(width * height);
		mVisible = std::make_unique<bool[]>(width * height);
	}

	Grid::Grid(Grid &&other)
		: mPoints(std::move(other.mPoints)), mColors(std::move(other.mColors)), mVisible(std::move(other.mVisible))
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

	const Geo::Vector &Grid::point(int x, int y) const
	{
		return mPoints[y * (mWidth + 1) + x];
	}

	void Grid::setPoint(int x, int y, const Geo::Vector &point)
	{
		mPoints[y * (mWidth + 1) + x] = point;
	}

	const Draw::Color &Grid::color(int x, int y) const
	{
		return mColors[y * mWidth + x];
	}

	void Grid::setColor(int x, int y, const Draw::Color &color)
	{
		mColors[y * mWidth + x] = color;
	}

	bool Grid::visible(int x, int y) const
	{
		return mVisible[y * mWidth + x];
	}

	void Grid::setVisible(int x, int y, bool visible)
	{
		mVisible[y * mWidth + x] = visible;
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

		for(int x = 0; x <= width(); x++) {
			for(int y = 0; y <= height(); y++) {
				if(x > 0) {
					Geo::Vector a = point(x - 1, y);
					Geo::Vector b = point(x, y);

					dc.aaline(a.x(), a.y(), b.x(), b.y(), Draw::Color(0.75f, 0.75f, 0.75f));
				}

				if(y > 0) {
					Geo::Vector a = point(x, y - 1);
					Geo::Vector b = point(x, y);

					dc.aaline(a.x(), a.y(), b.x(), b.y(), Draw::Color(0.75f, 0.75f, 0.75f));
				}
			}
		}
	}

	void Grid::renderSolid(const Config &config) const
	{
		for(int x = 0; x < width() - 1; x++) {
			for(int y = 0; y < height() - 1; y++) {
				if(!visible(x, y)) {
					continue;
				}

				Geo::Vector p0 = point(x, y);
				Draw::Color c0 = color(x, y);
				Geo::Vector p1 = point(x + 1, y);
				Draw::Color c1 = color(x + 1, y);
				Geo::Vector p2 = point(x + 1, y + 1);
				Draw::Color c2 = color(x + 1, y + 1);
				Geo::Vector p3 = point(x, y + 1);
				Draw::Color c3 = color(x, y + 1);

				Rasterize::renderQuad(config.framebuffer(), p0, c0, p1, c1, p2, c2, p3, c3, true);
			}
		}
	}
}