#ifndef BPT_FILE_LOADER_HPP
#define BPT_FILE_LOADER_HPP

#include "PatchSet.hpp"

#include <string>

class BptFileLoader
{
public:
	static PatchSet load(const std::string &filename);
};

#endif