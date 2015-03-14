#include "Framebuffer.hpp"

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

int Framebuffer::width()
{
	return mWidth;
}

int Framebuffer::height()
{
	return mHeight;
}

int Framebuffer::multisample()
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