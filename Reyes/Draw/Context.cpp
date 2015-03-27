#include "Draw/Context.hpp"

#include <utility>
#include <cmath>

namespace Draw {
	Context::Context(Framebuffer &framebuffer)
		: mFramebuffer(framebuffer)
	{
	}

	int Context::width()
	{
		return mFramebuffer.width();
	}

	int Context::height()
	{
		return mFramebuffer.height();
	}

	void Context::blendPixel(int x, int y, const Color &color, float alpha)
	{
		if(x < 0 || y < 0 || x >= mFramebuffer.width() || y >= mFramebuffer.height()) {
			return;
		}

		for(int m = 0; m < mFramebuffer.multisample(); m++) {
			Color current = mFramebuffer.getPixel(x, y, m);
			mFramebuffer.setPixel(x, y, m, current * (1.0f - alpha) + color * alpha);
		}
	}

	void Context::aaline(float x0, float y0, float x1, float y1, const Color &color)
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
			blendPixel(y - 1, x, color, (1.0f - err) * (1.0f - x0gap));
			blendPixel(y, x, color, err * (1.0f - x0gap));
		} else {
			blendPixel(x, y - 1, color, (1.0f - err) * (1.0f - x0gap));
			blendPixel(x, y, color, err * (1.0f - x0gap));
		}

		int xend = (int)std::floor(x1);
		while(x < xend - 1) {
			err += slope;
			if(err > 1.0f) {
				err -= 1.0f;
				y++;
			} else if(err < 0.0f) {
				err += 1.0f;
				y--;
			}
			x++;

			if(steep) {
				blendPixel(y - 1, x, color, (1.0f - err));
				blendPixel(y, x, color, err);
			} else {
				blendPixel(x, y - 1, color, (1.0f - err));
				blendPixel(x, y, color, err);
			}
		}

		float x1gap = x1 - xend;
		if(steep) {
			blendPixel(y - 1, x, color, (1.0f - err) * x1gap);
			blendPixel(y, x, color, err * x1gap);
		} else {
			blendPixel(x, y - 1, color, (1.0f - err) * x1gap);
			blendPixel(x, y, color, err * x1gap);
		}
	}
}