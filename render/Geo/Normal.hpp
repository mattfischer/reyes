#ifndef GEO_NORMAL_HPP
#define GEO_NORMAL_HPP

#include "Geo/Forwards.hpp"
#include "Geo/Coordinate.hpp"

namespace Geo {

class Normal : public Coordinate
{
public:
	Normal();
	Normal(const Normal &c);
	Normal(float x, float y, float z);
	explicit Normal(const Point &point);
	explicit Normal(const Coordinate &c);

	float operator*(const Normal &b) const;
	float operator*(const Vector &b) const;
	Normal operator-() const;
};

Normal operator*(const BaseTransformation &transformation, const Normal &normal);
float operator*(const Vector &vector, const Normal &normal);

}

#endif
