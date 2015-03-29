#ifndef RENDER_TEXTURE_HPP
#define RENDER_TEXTURE_HPP

#include "Draw/Color.hpp"

#include <memory>

namespace Render {
	class Texture
	{
	public:
		Texture(unsigned int width, unsigned int height);

		unsigned int width() const;
		unsigned int height() const;

		Draw::Color color(unsigned int x, unsigned int y) const;
		void setColor(unsigned int x, unsigned int y, const Draw::Color &color);

		Draw::Color sample(float s, float t) const;

	private:
		unsigned int mWidth;
		unsigned int mHeight;
		std::unique_ptr<unsigned char[]> mBits;
	};
}

#endif