#ifndef GEO_VECTOR_HPP
#define GEO_VECTOR_HPP

#include "Geo/Forwards.hpp"
#include "Geo/Coordinate.hpp"

namespace Geo {

class Vector : public Coordinate
{
public:
	Vector();
	Vector(const Vector &c);
	Vector(float x, float y, float z);
	explicit Vector(const Point &point);
	explicit Vector(const Normal &normal);
	explicit Vector(const Coordinate &c);

	Vector normalize() const;
	float magnitude() const;
	float magnitude2() const;

	Vector operator+(const Vector &b) const;
	Vector operator-(const Vector &b) const;
	float operator*(const Vector &b) const;
	Vector operator*(float b) const;
	Vector operator/(float b) const;
	Vector operator-() const;
};

Vector operator*(const BaseTransformation &transformation, const Vector &vector);

}

#endif
