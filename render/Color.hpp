#ifndef COLOR_HPP
#define COLOR_HPP

struct Color
{
	unsigned char r;
	unsigned char g;
	unsigned char b;

	Color(unsigned char _r, unsigned char _g, unsigned char _b) : r(_r), g(_g), b(_b) {}
};

#endif