#ifndef DRAW_FRAMEBUFFER_HPP
#define DRAW_FRAMEBUFFER_HPP

#include "Draw/Color.hpp"

#include <memory>

namespace Draw {
	class Framebuffer
	{
	public:
		Framebuffer();
		Framebuffer(int width, int height, int multisample);
		Framebuffer(Framebuffer &&other);
		Framebuffer(const Framebuffer &) = delete;

		Framebuffer &operator=(Framebuffer &&other);
		Framebuffer &operator=(const Framebuffer &) = delete;

		int width() const;
		int height() const;
		int multisample() const;

		const unsigned char *displayColorBits() const;

		void setPixel(int x, int y, int m, const Color &color);
		Color getPixel(int x, int y, int m) const;

		void setDepth(int x, int y, int m, float depth);
		float getDepth(int x, int y, int m) const;

		void postMultisampleBuffer();
		void clear(const Color &color);

	private:
		int mWidth;
		int mHeight;
		int mMultisample;
		std::unique_ptr<unsigned char[]> mDisplayColorBits;
		std::unique_ptr<unsigned char[]> mColorBits;
		std::unique_ptr<unsigned short[]> mDepthBits;
	};
}

#endif