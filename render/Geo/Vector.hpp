#ifndef GEO_VECTOR_HPP
#define GEO_VECTOR_HPP

namespace Geo {
	class Vector
	{
	public:
		Vector(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
		Vector(const Vector &v) = default;
		Vector &operator=(const Vector &v) = default;

		float x() const;
		void setX(float x);

		float y() const;
		void setY(float y);

		float z() const;
		void setZ(float z);

		float w() const;
		void setW(float w);

		float &at(int i);
		const float &at(int i) const;

		float &operator()(int i);
		const float &operator()(int i) const;

		Vector project() const;
		Vector normalize() const;
		float magnitude() const;

		Vector operator+(const Vector &b) const;
		Vector operator-(const Vector &b) const;
		float operator*(const Vector &b) const;
		Vector operator*(float b) const;
		Vector operator/(float b) const;
		Vector operator-() const;
		Vector operator%(const Vector &b) const;

		Vector &operator+=(const Vector &b);

	protected:
		float mValues[4];
	};

	Vector operator*(float b, const Vector &v);
}

#endif
