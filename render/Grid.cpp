#include "Grid.hpp"

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
