#include "DrawContext.hpp"

#include <utility>
#include <cmath>

DrawContext::DrawContext(Framebuffer &framebuffer)
	: mFramebuffer(framebuffer)
{
}

int DrawContext::width()
{
	return mFramebuffer.width();
}

int DrawContext::height()
{
	return mFramebuffer.height();
}

void DrawContext::setPixel(int x, int y, int m, const Color &color)
{
	unsigned char *bits = mFramebuffer.colorBits();
	int addr = ((y * mFramebuffer.width() + x) * mFramebuffer.multisample() + m) * 3;
	bits[addr + 0] = color.b;
	bits[addr + 1] = color.g;
	bits[addr + 2] = color.r;
}

void DrawContext::blendDisplayPixel(int x, int y, const Color &color, float alpha)
{
	if(x < 0 || y < 0 || x >= mFramebuffer.width() || y >= mFramebuffer.height()) {
		return;
	}

	unsigned char *bits = mFramebuffer.displayColorBits();
	int addr = (y * mFramebuffer.width() + x) * 3;
	bits[addr + 0] = unsigned char(bits[addr + 0] * (1.0f - alpha) + color.b * alpha);
	bits[addr + 1] = unsigned char(bits[addr + 1] * (1.0f - alpha) + color.g * alpha);
	bits[addr + 2] = unsigned char(bits[addr + 2] * (1.0f - alpha) + color.r * alpha);
}

void DrawContext::fillRect(int x, int y, int width, int height, const Color &color)
{
	for(int i = x; i < x + width; i++) {
		for(int j = y; j < y + height; j++) {
			for(int m = 0; m < mFramebuffer.multisample(); m++) {
				setPixel(i, j, m, color);
			}
		}
	}
}

void DrawContext::aaline(float x0, float y0, float x1, float y1, const Color &color)
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
		blendDisplayPixel(y - 1, x, color, (1.0f - err) * (1.0f - x0gap));
		blendDisplayPixel(y, x, color, err * (1.0f - x0gap));
	}
	else {
		blendDisplayPixel(x, y - 1, color, (1.0f - err) * (1.0f - x0gap));
		blendDisplayPixel(x, y, color, err * (1.0f - x0gap));
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
			blendDisplayPixel(y - 1, x, color, (1.0f - err));
			blendDisplayPixel(y, x, color, err);
		}
		else {
			blendDisplayPixel(x, y - 1, color, (1.0f - err));
			blendDisplayPixel(x, y, color, err);
		}
	}

	float x1gap = x1 - xend;
	if(steep) {
		blendDisplayPixel(y - 1, x, color, (1.0f - err) * x1gap);
		blendDisplayPixel(y, x, color, err * x1gap);
	}
	else {
		blendDisplayPixel(x, y - 1, color, (1.0f - err) * x1gap);
		blendDisplayPixel(x, y, color, err * x1gap);
	}
}

void DrawContext::setDepth(int x, int y, int m, unsigned short depth)
{
	mFramebuffer.depthBits()[(y * mFramebuffer.width() + x) * mFramebuffer.multisample() + m] = depth;
}

void DrawContext::fillRectDepth(int x, int y, int width, int height, unsigned short depth)
{
	for(int i = x; i < x + width; i++) {
		for(int j = y; j < y + height; j++) {
			for(int m = 0; m < mFramebuffer.multisample(); m++) {
				setDepth(i, j, m, depth);
			}
		}
	}
}

unsigned short DrawContext::getDepth(int x, int y, int m)
{
	return mFramebuffer.depthBits()[(y * mFramebuffer.width() + x) * mFramebuffer.multisample() + m];
}

void DrawContext::doMultisample()
{
	for(int x = 0; x < mFramebuffer.width(); x++) {
		for(int y = 0; y < mFramebuffer.height(); y++) {
			for(int c = 0; c < 3; c++) {
				unsigned int val = 0;
				for(int m = 0; m < mFramebuffer.multisample(); m++) {
					val += mFramebuffer.colorBits()[(((y * mFramebuffer.width()) + x) * mFramebuffer.multisample() + m) * 3 + c];
				}
				mFramebuffer.displayColorBits()[((y * mFramebuffer.width()) + x) * 3 + c] = val / mFramebuffer.multisample();
			}
		}
	}
}