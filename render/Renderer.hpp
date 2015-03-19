#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Framebuffer.hpp"
#include "Mesh.hpp"
#include "Geo/Matrix.hpp"

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
	void render();

private:
	Framebuffer &mFramebuffer;
	Mesh mMesh;
	Geo::Matrix mMatrices[unsigned int(MatrixType::Count)];
};

#endif