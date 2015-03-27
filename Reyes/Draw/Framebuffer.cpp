#include "Draw/Framebuffer.hpp"

#include <climits>

namespace Draw {
	Framebuffer::Framebuffer()
	{
		mWidth = 0;
		mHeight = 0;
		mMultisample = 0;
		mDisplayColorBits = 0;
		mColorBits = 0;
		mDepthBits = 0;
	}

	Framebuffer::Framebuffer(int width, int height, int multisample)
	{
		mWidth = width;
		mHeight = height;
		mMultisample = multisample;
		mDisplayColorBits = new unsigned char[mWidth * mHeight * 3];
		mColorBits = new unsigned char[mWidth * mHeight * mMultisample * 3];
		mDepthBits = new unsigned short[mWidth * mHeight * mMultisample];
	}

	Framebuffer::Framebuffer(Framebuffer &&other)
	{
		mWidth = other.mWidth;
		mHeight = other.mHeight;
		mMultisample = other.mMultisample;

		mDisplayColorBits = other.mDisplayColorBits;
		other.mDisplayColorBits = 0;
		mColorBits = other.mColorBits;
		other.mColorBits = 0;
		mDepthBits = other.mDepthBits;
		other.mDepthBits = 0;
	}

	Framebuffer::~Framebuffer()
	{
		if(mDisplayColorBits) {
			delete[] mDisplayColorBits;
		}

		if(mColorBits) {
			delete[] mColorBits;
		}

		if(mDepthBits) {
			delete[] mDepthBits;
		}
	}

	Framebuffer &Framebuffer::operator=(Framebuffer &&other)
	{
		mWidth = other.mWidth;
		mHeight = other.mHeight;
		mMultisample = other.mMultisample;

		if(mDisplayColorBits) {
			delete[] mDisplayColorBits;
		}
		mDisplayColorBits = other.mDisplayColorBits;
		other.mDisplayColorBits = 0;

		if(mColorBits) {
			delete[] mColorBits;
		}
		mColorBits = other.mColorBits;
		other.mColorBits = 0;

		if(mDepthBits) {
			delete[] mDepthBits;
		}
		mDepthBits = other.mDepthBits;
		other.mDepthBits = 0;

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
		return mDisplayColorBits;
	}

	unsigned char *Framebuffer::displayColorBits()
	{
		return mDisplayColorBits;
	}

	const unsigned char *Framebuffer::colorBits() const
	{
		return mColorBits;
	}

	unsigned char *Framebuffer::colorBits()
	{
		return mColorBits;
	}

	const unsigned short *Framebuffer::depthBits() const
	{
		return mDepthBits;
	}

	unsigned short *Framebuffer::depthBits()
	{
		return mDepthBits;
	}

	void Framebuffer::setPixel(int x, int y, int m, const Color &color)
	{
		unsigned char *bits = colorBits();
		int addr = ((y * width() + x) * multisample() + m) * 3;
		bits[addr + 0] = unsigned char(color.b * 0xff);
		bits[addr + 1] = unsigned char(color.g * 0xff);
		bits[addr + 2] = unsigned char(color.r * 0xff);
	}

	Color Framebuffer::getPixel(int x, int y, int m) const
	{
		const unsigned char *bits = colorBits();
		int addr = ((y * width() + x) * multisample() + m) * 3;
		float r = float(bits[addr + 0]) / 0xff;
		float g = float(bits[addr + 1]) / 0xff;
		float b = float(bits[addr + 2]) / 0xff;
		return Color(r, g, b);
	}

	void Framebuffer::setDepth(int x, int y, int m, float depth)
	{
		depthBits()[(y * width() + x) * multisample() + m] = unsigned short(depth * float(USHRT_MAX));
	}

	float Framebuffer::getDepth(int x, int y, int m) const
	{
		return float(depthBits()[(y * width() + x) * multisample() + m]) / float(USHRT_MAX);
	}

	void Framebuffer::postMultisampleBuffer()
	{
		for(int x = 0; x < width(); x++) {
			for(int y = 0; y < height(); y++) {
				for(int c = 0; c < 3; c++) {
					unsigned int val = 0;
					for(int m = 0; m < multisample(); m++) {
						val += colorBits()[(((y * width()) + x) * multisample() + m) * 3 + c];
					}
					displayColorBits()[((y * width()) + x) * 3 + c] = val / multisample();
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