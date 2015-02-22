#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

class Framebuffer
{
public:
	Framebuffer();
	Framebuffer(int width, int height);
	Framebuffer(Framebuffer &&other);
	Framebuffer(const Framebuffer &) = delete;
	~Framebuffer();

	Framebuffer &operator=(Framebuffer &&other);
	Framebuffer &operator=(const Framebuffer &) = delete;

	int width();
	int height();

	const unsigned char *bits() const;
	unsigned char *bits();

private:
	int mWidth;
	int mHeight;
	unsigned char *mBits;
};

#endif