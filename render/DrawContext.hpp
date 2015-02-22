#ifndef DRAW_CONTEXT_HPP
#define DRAW_CONTEXT_HPP

#include "Framebuffer.hpp"

class DrawContext
{
public:
	DrawContext(Framebuffer &framebuffer);

	struct Color {
		unsigned char r;
		unsigned char g;
		unsigned char b;

		Color(unsigned char _r, unsigned char _g, unsigned char _b) : r(_r), g(_g), b(_b) {}
	};

	void setPixel(int x, int y, Color &color);
	void blendPixel(int x, int y, Color &color, float alpha);
	void fillRect(int x, int y, int width, int height, Color &color);
	void aaline(float x0, float y0, float x1, float y1, Color &color);
private:
	Framebuffer &mFramebuffer;
};

#endif