#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Framebuffer.hpp"
#include "Geo/Matrix.hpp"
#include "Color.hpp"

#include <vector>

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

	struct Vertex
	{
		Geo::Vector position;
		Geo::Vector texCoord;
		Geo::Vector normal;

		Vertex() = default;
		Vertex(const Geo::Vector &_position, const Geo::Vector &_texCoord, const Geo::Vector &_normal) : position(_position), texCoord(_texCoord), normal(_normal) {}
	};

	struct Polygon
	{
		std::vector<Vertex> vertices;
		int numVertices;
	};

	void setMatrix(MatrixType type, const Geo::Matrix &modelView);
	const Geo::Matrix &matrix(MatrixType type);

	Framebuffer &framebuffer();

	void renderTriangle(const Vertex &p0, const Vertex &p1, const Vertex &p2, const Color &color);
	bool clipLine(Geo::Vector &a, Geo::Vector &b);
	bool clipPolygon(Polygon &polygon);

private:
	Framebuffer &mFramebuffer;
	Geo::Matrix mMatrices[unsigned int(MatrixType::Count)];
};

#endif