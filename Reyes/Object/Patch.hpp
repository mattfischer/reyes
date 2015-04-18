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

	private:
		virtual unsigned int numVaryingPoints() const;

		std::unique_ptr<Geo::Vector[]> mPoints;
	};
}

#endif