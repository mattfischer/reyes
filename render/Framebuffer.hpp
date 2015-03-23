#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "Color.hpp"

class Framebuffer
{
public:
	Framebuffer();
	Framebuffer(int width, int height, int multisample);
	Framebuffer(Framebuffer &&other);
	Framebuffer(const Framebuffer &) = delete;
	~Framebuffer();

	Framebuffer &operator=(Framebuffer &&other);
	Framebuffer &operator=(const Framebuffer &) = delete;

	int width() const;
	int height() const;
	int multisample() const;

	const unsigned char *displayColorBits() const;
	unsigned char *displayColorBits();

	const unsigned char *colorBits() const;
	unsigned char *colorBits();

	const unsigned short *depthBits() const;
	unsigned short *depthBits();

	void setPixel(int x, int y, int m, const Color &color);
	void setDepth(int x, int y, int m, unsigned short depth);
	unsigned short getDepth(int x, int y, int m) const;

	void postMultisampleBuffer();

private:
	int mWidth;
	int mHeight;
	int mMultisample;
	unsigned char *mDisplayColorBits;
	unsigned char *mColorBits;
	unsigned short *mDepthBits;
};

#endif