#ifndef BPT_FILE_LOADER_HPP
#define BPT_FILE_LOADER_HPP

#include "RenderObject.hpp"

#include <string>
#include <memory>

class BptFileLoader
{
public:
	static std::unique_ptr<RenderObject> load(const std::string &filename);
};

#endif