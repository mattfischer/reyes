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
	bool clipLine(Mesh::Vertex &a, Mesh::Vertex &b);
	bool clipToPlane(Mesh::Vertex &a, Mesh::Vertex &b, const Geo::Vector &normal);

	Mesh mMesh;
};

#endif