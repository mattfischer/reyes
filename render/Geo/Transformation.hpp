#ifndef GEO_TRANSFORMATION_HPP
#define GEO_TRANSFORMATION_HPP

#include "Geo/Matrix.hpp"

namespace Geo {

namespace Transformation
{
	Matrix translate(float x, float y, float z);
	Matrix translate(const Vector &vector);

	Matrix scale(float x, float y, float z);
	Matrix scale(const Vector &vector);

	Matrix uniformScale(float factor);

	Matrix rotate(float x, float y, float z);
	Matrix rotate(const Vector &vector);

	Matrix perspective(float width, float height, float near, float far);
	Matrix viewport(float xMin, float yMin, float xMax, float yMax);
};

}

#endif