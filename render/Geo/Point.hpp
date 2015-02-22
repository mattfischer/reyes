#ifndef GEO_POINT_HPP
#define GEO_POINT_HPP

#include "Geo/Forwards.hpp"
#include "Geo/Coordinate.hpp"

namespace Geo {

class Point : public Coordinate
{
public:
	Point();
	Point(float x, float y, float z);
	Point(const Point &c);
	explicit Point(const Coordinate &c);

	Point operator+(const Vector &b) const;
	Point operator-(const Vector &b) const;
	Vector operator-(const Point &b) const;
};

Point operator*(const BaseTransformation &transformation, const Point &point);

}

#endif
