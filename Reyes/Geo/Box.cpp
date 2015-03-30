#include "Geo/Box.hpp"

#include <algorithm>

namespace Geo {
	Box::Box()
	{
		for(int i = 0; i < 3; i++) {
			mMins[i] = FLT_MAX;
			mMaxes[i] = FLT_MIN;
		}
	}

	void Box::surround(const Vector &point)
	{
		for(int i = 0; i < 3; i++) {
			mMins[i] = std::min(mMins[i], point(i));
			mMaxes[i] = std::max(mMaxes[i], point(i));
		}
	}

	void Box::surround(const Box &box)
	{
		for(const Geo::Vector &point : box.points()) {
			surround(point);
		}
	}

	std::array<Vector, 8> Box::points() const
	{
		std::array<Vector, 8> points;
		for(int i = 0; i < 8; i++) {
			points[i].setX((i & 0x1) ? mMins[0] : mMaxes[0]);
			points[i].setY((i & 0x2) ? mMins[1] : mMaxes[1]);
			points[i].setZ((i & 0x4) ? mMins[2] : mMaxes[2]);
		}

		return points;
	}
}