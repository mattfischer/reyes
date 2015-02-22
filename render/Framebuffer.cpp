#include "Framebuffer.hpp"

Framebuffer::Framebuffer()
{
	mWidth = 0;
	mHeight = 0;
	mBits = 0;
}

Framebuffer::Framebuffer(int width, int height)
{
	mWidth = width;
	mHeight = height;
	mBits = new unsigned char[mWidth * mHeight * 3];
}

Framebuffer::Framebuffer(Framebuffer &&other)
{
	mWidth = other.mWidth;
	mHeight = other.mHeight;
	mBits = other.mBits;
	other.mBits = 0;
}

Framebuffer::~Framebuffer()
{
	if(mBits) {
		delete[] mBits;
	}
}

Framebuffer &Framebuffer::operator=(Framebuffer &&other)
{
	mWidth = other.mWidth;
	mHeight = other.mHeight;
	if(mBits) {
		delete[] mBits;
	}
	mBits = other.mBits;
	other.mBits = 0;

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

const unsigned char *Framebuffer::bits() const
{
	return mBits;
}

unsigned char *Framebuffer::bits()
{
	return mBits;
}