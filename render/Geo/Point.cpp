#include "Geo/Point.hpp"
#include "Geo/Vector.hpp"
#include "Geo/Transformation.hpp"

namespace Geo {
	
Point::Point()
{
}

Point::Point(float x, float y, float z)
: Coordinate(x, y, z, 1)
{
}

Point::Point(const Point &c)
: Coordinate(c)
{
}

Point::Point(const Coordinate &c)
: Coordinate(c)
{
}

Point Point::operator+(const Vector &b) const
{
	return Point(x() + b.x(), y() + b.y(), z() + b.z());
}

Point Point::operator-(const Vector &b) const
{
	return Point(x() - b.x(), y() - b.y(), z() - b.z());
}

Vector Point::operator-(const Point &b) const
{
	return Vector(x() - b.x(), y() - b.y(), z() - b.z());
}

Point operator*(const BaseTransformation &transformation, const Point &point)
{
	return Point(transformation.matrix() * point);
}

}