#ifndef GRID_HPP
#define GRID_HPP

#include "Geo/Vector.hpp"
#include "Object/Object.hpp"
#include "Draw/Color.hpp"

#include <memory>

namespace Render {
	class Grid
	{
	public:
		Grid(int width, int height);
		Grid(Grid &&other);

		Grid &operator=(Grid &&other);

		int width() const;
		int height() const;

		const Geo::Vector &point(int x, int y) const;
		void setPoint(int x, int y, const Geo::Vector &point);

		const Draw::Color &color(int x, int y) const;
		void setColor(int x, int y, const Draw::Color &color);

		bool visible(int x, int y) const;
		void setVisible(int x, int y, bool visible);

		virtual void render(const Config &config) const;

	private:
		void renderWireframe(const Config &config) const;
		void renderSolid(const Config &config) const;

		int mWidth;
		int mHeight;
		std::unique_ptr<Geo::Vector[]> mPoints;
		std::unique_ptr<Draw::Color[]> mColors;
		std::unique_ptr<bool[]> mVisible;
	};
}

#endif