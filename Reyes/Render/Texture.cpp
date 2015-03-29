#include "Render/Texture.hpp"

namespace Render {
	Texture::Texture(unsigned int width, unsigned int height)
	{
		mWidth = width;
		mHeight = height;
		mBits = std::make_unique<unsigned char[]>(mWidth * mHeight * 3);
	}

	unsigned int Texture::width() const
	{
		return mWidth;
	}

	unsigned int Texture::height() const
	{
		return mHeight;
	}

	Draw::Color Texture::color(unsigned int x, unsigned int y) const
	{
		Draw::Color color;
		color.r = float(mBits[(y * mWidth + x) * 3 + 0]) / 0xff;
		color.g = float(mBits[(y * mWidth + x) * 3 + 1]) / 0xff;
		color.b = float(mBits[(y * mWidth + x) * 3 + 2]) / 0xff;

		return color;
	}

	void Texture::setColor(unsigned int x, unsigned int y, const Draw::Color &color)
	{
		mBits[(y * mWidth + x) * 3 + 0] = unsigned char(color.r * 0xff);
		mBits[(y * mWidth + x) * 3 + 1] = unsigned char(color.g * 0xff);
		mBits[(y * mWidth + x) * 3 + 2] = unsigned char(color.b * 0xff);
	}

	Draw::Color Texture::sample(float s, float t) const
	{
		unsigned int x0 = unsigned int(std::floor(s * mWidth));
		unsigned int y0 = unsigned int(std::floor(t * mHeight));
		float xf = s * mWidth - x0;
		float yf = t * mHeight - y0;

		x0 = x0 % mWidth;
		if(x0 < 0) {
			x0 += mWidth;
		}

		y0 = y0 % mHeight;
		if(y0 < 0) {
			y0 += mHeight;
		}

		unsigned int x1 = (x0 + 1) % mWidth;
		unsigned int y1 = (y0 + 1) % mHeight;

		return color(x0, y0) * (1 - xf) * (1 - yf) + color(x1, y0) * xf * (1 - yf) + color(x0, y1) * (1 - xf) * yf + color(x1, y1) * xf * yf;
	}
}