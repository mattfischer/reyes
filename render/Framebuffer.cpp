#include "Framebuffer.hpp"

Framebuffer::Framebuffer()
{
	mWidth = 0;
	mHeight = 0;
	mColorBits = 0;
}

Framebuffer::Framebuffer(int width, int height)
{
	mWidth = width;
	mHeight = height;
	mColorBits = new unsigned char[mWidth * mHeight * 3];
	mDepthBits = new unsigned short[mWidth * mHeight];
}

Framebuffer::Framebuffer(Framebuffer &&other)
{
	mWidth = other.mWidth;
	mHeight = other.mHeight;
	mColorBits = other.mColorBits;
	other.mColorBits = 0;
	mDepthBits = other.mDepthBits;
	other.mDepthBits = 0;
}

Framebuffer::~Framebuffer()
{
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

int Framebuffer::width()
{
	return mWidth;
}

int Framebuffer::height()
{
	return mHeight;
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