#include "PatchSet.hpp"

PatchSet::PatchSet(std::vector<Patch> &&patches)
	: mPatches(std::move(patches))
{
}

PatchSet &PatchSet::operator=(PatchSet &&other)
{
	mPatches = std::move(other.mPatches);
	return *this;
}

PatchSet::const_iterator PatchSet::begin() const
{
	return mPatches.begin();
}

PatchSet::const_iterator PatchSet::end() const
{
	return mPatches.end();
}
