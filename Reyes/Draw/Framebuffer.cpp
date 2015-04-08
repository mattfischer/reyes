#include "Draw/Framebuffer.hpp"

#include <climits>

namespace Draw {
	Framebuffer::Framebuffer()
	{
		mWidth = 0;
		mHeight = 0;
		mMultisample = 0;
	}

	Framebuffer::Framebuffer(int width, int height, int multisample)
	{
		mWidth = width;
		mHeight = height;
		mMultisample = multisample;
		mDisplayColorBits = std::make_unique<unsigned char[]>(mWidth * mHeight * 3);
		mColorBits = std::make_unique<unsigned char[]>(mWidth * mHeight * mMultisample * 3);
		mDepthBits = std::make_unique<unsigned short[]>(mWidth * mHeight * mMultisample);
	}

	Framebuffer::Framebuffer(Framebuffer &&other)
	{
		mWidth = other.mWidth;
		mHeight = other.mHeight;
		mMultisample = other.mMultisample;

		mDisplayColorBits = std::move(other.mDisplayColorBits);
		mColorBits = std::move(other.mColorBits);
		mDepthBits = std::move(other.mDepthBits);
	}

	Framebuffer &Framebuffer::operator=(Framebuffer &&other)
	{
		mWidth = other.mWidth;
		mHeight = other.mHeight;
		mMultisample = other.mMultisample;

		mDisplayColorBits = std::move(other.mDisplayColorBits);
		mColorBits = std::move(other.mColorBits);
		mDepthBits = std::move(other.mDepthBits);

		return *this;
	}

	int Framebuffer::width() const
	{
		return mWidth;
	}

	int Framebuffer::height() const
	{
		return mHeight;
	}

	int Framebuffer::multisample() const
	{
		return mMultisample;
	}

	const unsigned char *Framebuffer::displayColorBits() const
	{
		return mDisplayColorBits.get();
	}

	void Framebuffer::setPixel(int x, int y, int m, const Color &color)
	{
		unsigned char *bits = mColorBits.get();
		int addr = ((y * width() + x) * multisample() + m) * 3;
		bits[addr + 0] = unsigned char(color.b * 0xff);
		bits[addr + 1] = unsigned char(color.g * 0xff);
		bits[addr + 2] = unsigned char(color.r * 0xff);
	}

	Color Framebuffer::getPixel(int x, int y, int m) const
	{
		const unsigned char *bits = mColorBits.get();
		int addr = ((y * width() + x) * multisample() + m) * 3;
		float r = float(bits[addr + 0]) / 0xff;
		float g = float(bits[addr + 1]) / 0xff;
		float b = float(bits[addr + 2]) / 0xff;
		return Color(r, g, b);
	}

	void Framebuffer::setDepth(int x, int y, int m, float depth)
	{
		mDepthBits[(y * width() + x) * multisample() + m] = unsigned short(depth * float(USHRT_MAX));
	}

	float Framebuffer::getDepth(int x, int y, int m) const
	{
		return float(mDepthBits[(y * width() + x) * multisample() + m]) / float(USHRT_MAX);
	}

	void Framebuffer::postMultisampleBuffer()
	{
		for(int x = 0; x < width(); x++) {
			for(int y = 0; y < height(); y++) {
				for(int c = 0; c < 3; c++) {
					unsigned int val = 0;
					for(int m = 0; m < multisample(); m++) {
						val += mColorBits[(((y * width()) + x) * multisample() + m) * 3 + c];
					}
					mDisplayColorBits[((y * width()) + x) * 3 + c] = val / multisample();
				}
			}
		}
	}

	void Framebuffer::clear(const Color &color)
	{
		for(int i = 0; i < width(); i++) {
			for(int j = 0; j < height(); j++) {
				for(int m = 0; m < multisample(); m++) {
					setPixel(i, j, m, color);
					setDepth(i, j, m, 1.0f);
				}
			}
		}
	}
}