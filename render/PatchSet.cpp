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

void PatchSet::renderWireframe(Renderer &renderer)
{
	for(Patch &patch : mPatches) {
		patch.renderWireframe(renderer);
	}
}

void PatchSet::renderSolid(Renderer &renderer)
{
	for(Patch &patch : mPatches) {
		patch.renderSolid(renderer);
	}
}
