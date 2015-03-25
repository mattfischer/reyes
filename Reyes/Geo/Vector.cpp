#include "Geo/Vector.hpp"

#include <cmath>

namespace Geo {
	Vector::Vector(float x, float y, float z, float w)
	{
		mValues[0] = x;
		mValues[1] = y;
		mValues[2] = z;
		mValues[3] = w;
	}

	float Vector::x() const
	{
		return mValues[0];
	}

	void Vector::setX(float x)
	{
		mValues[0] = x;
	}

	float Vector::y() const
	{
		return mValues[1];
	}

	void Vector::setY(float y)
	{
		mValues[1] = y;
	}

	float Vector::z() const
	{
		return mValues[2];
	}

	void Vector::setZ(float z)
	{
		mValues[2] = z;
	}

	float Vector::w() const
	{
		return mValues[3];
	}

	void Vector::setW(float z)
	{
		mValues[3] = z;
	}

	float &Vector::at(int i)
	{
		return mValues[i];
	}

	const float &Vector::at(int i) const
	{
		return mValues[i];
	}

	float &Vector::operator()(int i)
	{
		return at(i);
	}

	const float &Vector::operator()(int i) const
	{
		return at(i);
	}

	Vector Vector::operator+(const Vector &b) const
	{
		return Vector(x() + b.x(), y() + b.y(), z() + b.z(), w() + b.w());
	}

	Vector Vector::operator-(const Vector &b) const
	{
		return Vector(x() - b.x(), y() - b.y(), z() - b.z(), w() - b.w());
	}

	float Vector::operator*(const Vector &b) const
	{
		return x() * b.x() + y() * b.y() + z() * b.z() + w() * b.w();
	}

	Vector Vector::operator*(float b) const
	{
		return Vector(x() * b, y() * b, z() * b, w() * b);
	}

	Vector Vector::operator/(float b) const
	{
		return Vector(x() / b, y() / b, z() / b, w() / b);
	}

	Vector Vector::operator-() const
	{
		return *this * -1;
	}

	Vector Vector::project() const
	{
		return Vector(x() / w(), y() / w(), z() / w(), 1.0f);
	}

	float Vector::magnitude() const
	{
		return std::sqrt(x() * x() + y() * y() + z() * z());
	}

	Vector Vector::normalize() const
	{
		return *this / magnitude();
	}

	Vector Vector::operator%(const Vector &b) const
	{
		return Vector(y() * b.z() - b.y() * z(), z() * b.x() - b.z() * x(), x() * b.y() - b.x() * y());
	}

	Vector operator*(float b, const Vector &v)
	{
		return v * b;
	}

	Vector &Vector::operator+=(const Vector &b)
	{
		for(int i = 0; i < 4; i++) {
			mValues[i] += b.mValues[i];
		}

		return *this;
	}
}