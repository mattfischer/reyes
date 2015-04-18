#ifndef BPT_FILE_LOADER_HPP
#define BPT_FILE_LOADER_HPP

#include "Render/RenderableObject.hpp"
#include "Render/Texture.hpp"

#include <string>
#include <memory>

class BptFileLoader
{
public:
	static std::unique_ptr<Render::RenderableObject> load(const std::string &filename, Render::Texture &texture);
};

#endif