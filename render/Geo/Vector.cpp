#include "Geo/Vector.hpp"
#include "Geo/Point.hpp"
#include "Geo/Transformation.hpp"
#include "Geo/Normal.hpp"

#include <cmath>

namespace Geo {

Vector::Vector()
{
}

Vector::Vector(float x, float y, float z)
: Coordinate(x, y, z, 0)
{
}

Vector::Vector(const Vector &c)
: Coordinate(c)
{
}

Vector::Vector(const Coordinate &c)
: Coordinate(c)
{
}

Vector::Vector(const Point &point)
: Coordinate(point)
{
}

Vector::Vector(const Normal &normal)
: Coordinate(normal)
{
}

float Vector::magnitude() const
{
	return std::sqrt(magnitude2());
}

float Vector::magnitude2() const
{
	return *this * *this;
}

Vector Vector::normalize() const
{
	float m = magnitude();

	return Vector(x() / m, y() / m, z() / m);
}

Vector Vector::operator+(const Vector &b) const
{
	return Vector(x() + b.x(), y() + b.y(), z() + b.z());
}

Vector Vector::operator-(const Vector &b) const
{
	return Vector(x() - b.x(), y() - b.y(), z() - b.z());
}

float Vector::operator*(const Vector &b) const
{
	return x() * b.x() + y() * b.y() + z() * b.z();
}

Vector Vector::operator*(float b) const
{
	return Vector(x() * b, y() * b, z() * b);
}

Vector Vector::operator/(float b) const
{
	return Vector(x() / b, y() / b, z() / b);
}

Vector Vector::operator-() const
{
	return *this * -1;
}

Vector operator*(const BaseTransformation &transformation, const Vector &vector)
{

	return Vector(transformation.matrix() * vector);
}

}