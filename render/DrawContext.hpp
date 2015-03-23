#ifndef DRAW_CONTEXT_HPP
#define DRAW_CONTEXT_HPP

#include "Framebuffer.hpp"
#include "Color.hpp"

class DrawContext
{
public:
	DrawContext(Framebuffer &framebuffer);

	int width();
	int height();

	void blendDisplayPixel(int x, int y, const Color &color, float alpha);
	void fillRect(int x, int y, int width, int height, const Color &color);
	void aaline(float x0, float y0, float x1, float y1, const Color &color);

	void fillRectDepth(int x, int y, int width, int height, unsigned short depth);

private:
	Framebuffer &mFramebuffer;
};

#endif