#ifndef MESH_HPP
#define MESH_HPP

#include "Geo/Vector.hpp"
#include "Draw/Color.hpp"
#include "RenderObject.hpp"

#include <vector>
#include <memory>
#include <tuple>

class Mesh : public RenderObject
{
public:
	typedef std::tuple<int, int> Edge;

	struct Vertex
	{
		Geo::Vector position;
		Geo::Vector texCoord;
		Geo::Vector normal;

		Vertex() = default;
		Vertex(const Geo::Vector &_position, const Geo::Vector &_texCoord, const Geo::Vector &_normal) : position(_position), texCoord(_texCoord), normal(_normal) {}
	};

	struct Polygon {
		std::vector<int> indices;
		Draw::Color color;
		int texture;

		Polygon(std::initializer_list<int> _indices, Draw::Color &_color, int _texture) : indices(_indices), color(_color), texture(_texture) {}
	};

	struct Texture
	{
		int width;
		int height;
		std::vector<Draw::Color> data;

		Texture(int _width, int _height, std::vector<Draw::Color> &&_data) : data(std::move(_data)), width(_width), height(_height) {}
	};

	Mesh() = default;
	Mesh(std::vector<Vertex> &&vertices, std::vector<Edge> &&edges, std::vector<Polygon> &&polygons, std::vector<Texture> &&textures);

	const std::vector<Vertex> &vertices() const;
	const std::vector<Edge> &edges() const;
	const std::vector<Polygon> &polygons() const;
	const std::vector<Texture> &textures() const;

	virtual void render(const RenderConfig &config) const;

private:
	void renderWireframe(const RenderConfig &config) const;
	void renderSolid(const RenderConfig &renderer) const;

	std::vector<Vertex> mVertices;
	std::vector<Edge> mEdges;
	std::vector<Polygon> mPolygons;
	std::vector<Texture> mTextures;
};

#endif