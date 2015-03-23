#ifndef GRID_HPP
#define GRID_HPP

#include "Geo/Vector.hpp"
#include "RenderObject.hpp"

#include <memory>

class Grid : public RenderObject
{
public:
	struct Point
	{
		Geo::Vector position;
		Geo::Vector texCoord;
		Geo::Vector normal;

		Point() = default;
		Point(const Geo::Vector &_position, const Geo::Vector &_texCoord, Geo::Vector &_normal) : position(_position), texCoord(_texCoord), normal(_normal) {}
	};

	Grid(int width, int height);
	Grid(Grid &&other);

	Grid &operator=(Grid &&other);

	int width() const;
	int height() const;

	void setPoint(int x, int y, const Point &point);
	const Point &point(int x, int y) const;

	virtual void render(const RenderConfig &config) const;

private:
	void renderWireframe(const RenderConfig &config) const;
	void renderSolid(const RenderConfig &config) const;

	int mWidth;
	int mHeight;
	std::unique_ptr<Point[]> mPoints;
};
#endif