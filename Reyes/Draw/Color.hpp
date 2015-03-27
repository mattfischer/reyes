#ifndef DRAW_COLOR_HPP
#define DRAW_COLOR_HPP

namespace Draw {
	struct Color
	{
		float r;
		float g;
		float b;

		Color operator*(float c) const
		{
			return Color(r * c, g * c, b * c);
		}

		Color operator+(const Color &other) const
		{
			return Color(r + other.r, g + other.g, b + other.b);
		}

		Color() = default;
		Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
	};

	static Color operator*(float f, const Color &c)
	{
		return c * f;
	}
}

#endif