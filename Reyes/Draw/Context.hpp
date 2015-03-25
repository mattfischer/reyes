#ifndef DRAW_CONTEXT_HPP
#define DRAW_CONTEXT_HPP

#include "Draw/Framebuffer.hpp"
#include "Draw/Color.hpp"

namespace Draw {
	class Context
	{
	public:
		Context(Framebuffer &framebuffer);

		int width();
		int height();

		void blendDisplayPixel(int x, int y, const Color &color, float alpha);
		void aaline(float x0, float y0, float x1, float y1, const Color &color);

	private:
		Framebuffer &mFramebuffer;
	};
}

#endif