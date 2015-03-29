#include "BmpFileLoader.hpp"

#include <fstream>

#include <windows.h>

std::unique_ptr<Render::Texture> BmpFileLoader::load(const std::string &filename)
{
	std::ifstream file(filename.c_str(), std::ios_base::binary);

	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	file.read((char*)&bfh, sizeof(bfh));
	file.read((char*)&bih, sizeof(bih));

	std::unique_ptr<Render::Texture> texture = std::make_unique<Render::Texture>(bih.biWidth, bih.biHeight);
	file.seekg(bfh.bfOffBits);

	unsigned int size = bih.biWidth * bih.biHeight * 3;
	std::unique_ptr<unsigned char[]> bits = std::make_unique<unsigned char[]>(size);
	file.read((char*)bits.get(), size);
	for(unsigned int x=0; x < texture->width(); x++) {
		for(unsigned int y=0; y < texture->height(); y++) {
			unsigned char r = bits[(y * texture->width() + x) * 3 + 2];
			unsigned char g = bits[(y * texture->width() + x) * 3 + 1];
			unsigned char b = bits[(y * texture->width() + x) * 3 + 0];
			texture->setColor(x, y, Draw::Color(float(r) / 0xff, float(g) / 0xff, float(b) / 0xff));
		}
	}

	return texture;
}