#ifndef COLOR_HPP
#define COLOR_HPP

struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;

	Color operator*(float c) const
	{
		return Color(unsigned char(r * c), unsigned char(g * c), unsigned char(b * c));
	}

	Color operator+(const Color &other) const
	{
		return Color(r + other.r, g + other.g, b + other.b);
	}

	Color(unsigned char _r, unsigned char _g, unsigned char _b) : r(_r), g(_g), b(_b) {}
};

#endif