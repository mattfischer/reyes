#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Framebuffer.hpp"
#include "Mesh.hpp"
#include "Geo/Matrix.hpp"
#include "PatchSet.hpp"

class Renderer
{
public:
	Renderer(Framebuffer &framebuffer);
	Renderer &operator=(Renderer &other) = default;

	enum class MatrixType
	{
		ModelView,
		Projection,
		Viewport,
		Count
	};

	void setMatrix(MatrixType type, const Geo::Matrix &modelView);
	const Geo::Matrix &matrix(MatrixType type);

	void renderMeshWireframe(const Mesh &mesh);
	void renderMeshPolygons(const Mesh &mesh);
	void renderPatchSetPolygons(const PatchSet &patchSet);
	void renderPatchSetWireframe(const PatchSet &patchSet);

	void render();

private:
	Framebuffer &mFramebuffer;
	PatchSet mPatchSet;
	Geo::Matrix mMatrices[unsigned int(MatrixType::Count)];
};

#endif