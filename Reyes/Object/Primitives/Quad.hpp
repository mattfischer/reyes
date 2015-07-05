#ifndef OBJECT_PRIMITIVES_QUAD_HPP
#define OBJECT_PRIMITIVES_QUAD_HPP

#include "Object/Primitive.hpp"

#include "Geo/Vector.hpp"

namespace Object {
	namespace Primitives {
		class Quad : public Primitive
		{
		public:
			Quad(Render::Texture &texture, const Geo::Vector &origin, const Geo::Vector &u, const Geo::Vector &v);

		protected:
			virtual Render::Grid dice(const Segment &segment, const Render::Config &config) const;
			virtual bool canDice(const Segment &segment, const Render::Config &config) const;
			virtual void splitU(const Segment &segment, Segment &a, Segment &b) const;
			virtual void splitV(const Segment &segment, Segment &a, Segment &b) const;

		private:
			Geo::Vector mOrigin;
			Geo::Vector mU;
			Geo::Vector mV;
		};
	}
}

#endif