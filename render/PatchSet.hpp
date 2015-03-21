#ifndef PATCH_SET_HPP
#define PATCH_SET_HPP

#include "Patch.hpp"
#include "Renderer.hpp"

#include <vector>

class PatchSet
{
public:
	PatchSet() = default;
	PatchSet(std::vector<Patch> &&patches);
	
	PatchSet &operator=(PatchSet &&other);

	void renderWireframe(Renderer &renderer);
	void renderSolid(Renderer &renderer);

private:
	std::vector<Patch> mPatches;
};

#endif