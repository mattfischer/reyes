#ifndef GEO_BOX_HPP
#define GEO_BOX_HPP

#include "Geo/Vector.hpp"

#include <array>

namespace Geo {
	class Box
	{
	public:
		Box();

		void surround(const Vector &point);
		void surround(const Box &box);

		std::array<Vector, 8> points() const;

	private:
		float mMins[3];
		float mMaxes[3];
	};
}

#endif