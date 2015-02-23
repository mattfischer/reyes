#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Framebuffer.hpp"
#include "Mesh.hpp"

class Renderer
{
public:
	Renderer();

	void render(Framebuffer &framebuffer);

private:
	Mesh mMesh;
};

#endif