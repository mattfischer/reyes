#ifndef OBJECT_PATCH_HPP
#define OBJECT_PATCH_HPP

#include "Geo/Vector.hpp"

#include "Object/Primitive.hpp"

#include "Render/Grid.hpp"
#include "Render/Texture.hpp"

#include "Draw/Color.hpp"

#include <memory>

namespace Object {
	class Patch : public Primitive
	{
	public:
		Patch(Geo::Vector points[16], Render::Texture &texture);
		Patch(Patch &&other);

		const Geo::Vector &point(int x, int y) const;

	protected:
		virtual Render::Grid dice(const Segment &segment, const Render::Config &config) const;
		virtual bool canDice(const Segment &segment, const Render::Config &config) const;
		virtual void splitU(const Segment &segment, Segment &a, Segment &b) const;
		virtual void splitV(const Segment &segment, Segment &a, Segment &b) const;

	private:
		virtual unsigned int numVaryingPoints() const;

		void computeHull(float uMin, float vMin, float uMax, float vMax, Geo::Vector points[16]) const;

		std::unique_ptr<Geo::Vector[]> mPoints;
	};
}

#endif