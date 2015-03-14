#include "Geo/Transformation.hpp"
#include "Geo/Vector.hpp"

#include <cmath>

#pragma warning( disable : 4355 )

using std::sin;
using std::cos;

namespace Geo {
	Matrix Transformation::translate(const Vector &vector)
	{
		return translate(vector.x(), vector.y(), vector.z());
	}

	Matrix Transformation::translate(float x, float y, float z)
	{
		return Matrix(
			1, 0, 0, x,
			0, 1, 0, y,
			0, 0, 1, z,
			0, 0, 0, 1);
	}

	Matrix Transformation::scale(const Vector &vector)
	{
		return scale(vector.x(), vector.y(), vector.z());
	}

	Matrix Transformation::scale(float x, float y, float z)
	{
		return Matrix(
			x, 0, 0, 0,
			0, y, 0, 0,
			0, 0, z, 0,
			0, 0, 0, 1);
	}

	Matrix Transformation::uniformScale(float factor)
	{
		return scale(factor, factor, factor);
	}

	static float rad(float deg)
	{
		return deg * 3.14f / 180.0f;
	}

	Matrix Transformation::rotate(const Vector &vector)
	{
		return rotate(vector.x(), vector.y(), vector.z());
	}

	Matrix Transformation::rotate(float x, float y, float z)
	{
		float rx = rad(x);
		float ry = rad(y);
		float rz = rad(z);

		Matrix xRotate(
			1, 0, 0, 0,
			0, cos(rx), -sin(rx), 0,
			0, sin(rx), cos(rx), 0,
			0, 0, 0, 1);

		Matrix yRotate(
			cos(ry), 0, -sin(ry), 0,
			0, 1, 0, 0,
			sin(ry), 0, cos(ry), 0,
			0, 0, 0, 1);

		Matrix zRotate(
			cos(rz), sin(rz), 0, 0,
			-sin(rz), cos(rz), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		return xRotate * yRotate * zRotate;
	}

	Matrix Transformation::perspective(float width, float height, float near, float far)
	{
		return Matrix(
			2.0f / width, 0, 0, 0,
			0, 2.0f / height, 0, 0,
			0, 0, (far + near) / (far - near), -2.0f * near * far / (far - near),
			0, 0, 1, 0);
	}

	Matrix Transformation::viewport(float xMin, float yMin, float xMax, float yMax, float zMin, float zMax)
	{
		return Matrix(
			(xMax - xMin) / 2.0f, 0, 0, xMin + (xMax - xMin) / 2.0f,
			0, (yMax - yMin) / 2.0f, 0, yMin + (yMax - yMin) / 2.0f,
			0, 0, (zMax - zMin) / 2.0f, zMin + (zMax - zMin) / 2.0f,
			0, 0, 0, 1);
	}
}