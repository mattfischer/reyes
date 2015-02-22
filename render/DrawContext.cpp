#include "DrawContext.hpp"

#include <utility>
#include <cmath>

DrawContext::DrawContext(Framebuffer &framebuffer)
	: mFramebuffer(framebuffer)
{
}

void DrawContext::setPixel(int x, int y, Color &color)
{
	unsigned char *bits = mFramebuffer.bits();
	int addr = (y * mFramebuffer.width() + x) * 3;
	bits[addr + 0] = color.b;
	bits[addr + 1] = color.g;
	bits[addr + 2] = color.r;
}

void DrawContext::blendPixel(int x, int y, Color &color, float alpha)
{
	unsigned char *bits = mFramebuffer.bits();
	int addr = (y * mFramebuffer.width() + x) * 3;
	bits[addr + 0] = unsigned char(bits[addr + 0] * (1.0f - alpha) + color.b * alpha);
	bits[addr + 1] = unsigned char(bits[addr + 1] * (1.0f - alpha) + color.g * alpha);
	bits[addr + 2] = unsigned char(bits[addr + 2] * (1.0f - alpha) + color.r * alpha);
}

void DrawContext::fillRect(int x, int y, int width, int height, Color &color)
{
	for(int i = x; i < x + width; i++) {
		for(int j = y; j < y + height; j++) {
			setPixel(i, j, color);
		}
	}
}

void DrawContext::aaline(float x0, float y0, float x1, float y1, Color &color)
{
	bool steep = false;
	if(std::abs(y1 - y0) > std::abs(x1 - x0)) {
		steep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}

	if(x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	float slope = (y1 - y0) / (x1 - x0);
	int xstart = (int)std::floor(x0);
	float x0gap = x0 - xstart;
	float y0start = y0 - slope * (x0gap - 0.5f);
	
	int x = xstart;
	int y = (int)std::round(y0start);
	
	float err = y0start - ((float)y - 0.5f);
	if(steep) {
		blendPixel(y, x, color, (1.0f - err) * (1.0f - x0gap));
		blendPixel(y + 1, x, color, err * (1.0f - x0gap));
	}
	else {
		blendPixel(x, y, color, (1.0f - err) * (1.0f - x0gap));
		blendPixel(x, y + 1, color, err * (1.0f - x0gap));
	}

	int xend = (int)std::floor(x1);
	while(x < xend - 1) {
		err += slope;
		if(err > 1.0f) {
			err -= 1.0f;
			y++;
		}
		else if(err < 0.0f) {
			err += 1.0f;
			y--;
		}
		x++;

		if(steep) {
			blendPixel(y, x, color, (1.0f - err));
			blendPixel(y + 1, x, color, err);
		}
		else {
			blendPixel(x, y, color, (1.0f - err));
			blendPixel(x, y + 1, color, err);
		}
	}

	float x1gap = x1 - xend;
	if(steep) {
		blendPixel(y, x, color, (1.0f - err) * x1gap);
		blendPixel(y + 1, x, color, err * x1gap);
	}
	else {
		blendPixel(x, y, color, (1.0f - err) * x1gap);
		blendPixel(x, y + 1, color, err * x1gap);
	}
}