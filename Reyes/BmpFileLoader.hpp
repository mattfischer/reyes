#ifndef BMP_FILE_LOADER_HPP
#define BMP_FILE_LOADER_HPP

#include "Render/Texture.hpp"

#include <memory>
#include <string>

class BmpFileLoader
{
public:
	static std::unique_ptr<Render::Texture> load(const std::string &filename);
};

#endif