#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

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

	int width();
	int height();
	int multisample();

	const unsigned char *displayColorBits() const;
	unsigned char *displayColorBits();

	const unsigned char *colorBits() const;
	unsigned char *colorBits();

	const unsigned short *depthBits() const;
	unsigned short *depthBits();

private:
	int mWidth;
	int mHeight;
	int mMultisample;
	unsigned char *mDisplayColorBits;
	unsigned char *mColorBits;
	unsigned short *mDepthBits;
};

#endif