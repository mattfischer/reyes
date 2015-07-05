#include "Object/Primitives/Quad.hpp"

namespace Object {
	namespace Primitives {
		Quad::Quad(Render::Texture &texture, const Geo::Vector &origin, const Geo::Vector &u, const Geo::Vector &v)
			: Primitive(texture)
		{
			mOrigin = origin;
			mU = u;
			mV = v;

			boundingBox().surround(mOrigin);
			boundingBox().surround(mOrigin + mU);
			boundingBox().surround(mOrigin + mV);
			boundingBox().surround(mOrigin + mU + mV);
		}

		Render::Grid Quad::dice(const Segment &segment, const Render::Config &config) const
		{
			Geo::Matrix matrix = config.view() * transformation();

			Geo::Vector origin = matrix * mOrigin;
			Geo::Vector u = matrix * mU;
			Geo::Vector v = matrix * mV;

			Render::Grid grid(16, 16);
			for(int i = 0; i <= 16; i++) {
				for(int j = 0; j <= 16; j++) {
					grid.setPoint(i, j, origin + ((float)i / 16.0f) * u + ((float)j / 16.0f) * v);
				}
			}

			return grid;
		}

		bool Quad::canDice(const Segment &segment, const Render::Config &config) const
		{
			return true;
		}

		void Quad::splitU(const Segment &segment, Segment &a, Segment &b) const
		{
		}

		void Quad::splitV(const Segment &segment, Segment &a, Segment &b) const
		{
		}
	}
}