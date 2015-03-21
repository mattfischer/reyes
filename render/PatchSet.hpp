#ifndef PATCH_SET_HPP
#define PATCH_SET_HPP

#include "Patch.hpp"

#include <vector>

class PatchSet
{
public:
	PatchSet() = default;
	PatchSet(std::vector<Patch> &&patches);
	
	PatchSet &operator=(PatchSet &&other);

	typedef std::vector<Patch>::iterator iterator;
	typedef std::vector<Patch>::const_iterator const_iterator;
	const_iterator begin() const;
	const_iterator end() const;

private:
	std::vector<Patch> mPatches;
};

#endif