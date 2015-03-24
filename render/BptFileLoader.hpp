#ifndef BPT_FILE_LOADER_HPP
#define BPT_FILE_LOADER_HPP

#include "Render/Object.hpp"
#include "Draw/Color.hpp"

#include <string>
#include <memory>

class BptFileLoader
{
public:
	static std::unique_ptr<Render::Object> load(const std::string &filename);
};

#endif