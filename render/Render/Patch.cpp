#include "Render/Patch.hpp"

#include <algorithm>

namespace Render {
	Patch::Patch(Geo::Vector points[16])
	{
		mPoints = std::make_unique<Geo::Vector[]>(16);

		for(int i = 0; i < 16; i++) {
			mPoints[i] = points[i];
		}
	}

	Patch::Patch(Patch &&other)
		: mPoints(std::move(other.mPoints))
	{
	}

	const Geo::Vector &Patch::point(int x, int y) const
	{
		return mPoints[y * 4 + x];
	}

	Grid Patch::dice(const Config &config) const
	{
		int divisions = 16;
		Grid grid(divisions, divisions);

		Geo::Matrix matrix = config.view() * transformation();
		Geo::Vector points[16];
		for(int k = 0; k < 4; k++) {
			for(int l = 0; l < 4; l++) {
				points[k * 4 + l] = matrix * point(l, k);
			}
		}

		for(int i = 0; i <= divisions; i++) {
			for(int j = 0; j <= divisions; j++) {
				float s = float(i) / float(divisions);
				float t = float(j) / float(divisions);

				float Bs[4] = { (1 - s)*(1 - s)*(1 - s), 3 * s*(1 - s)*(1 - s), 3 * s*s*(1 - s), s*s*s };
				float Bt[4] = { (1 - t)*(1 - t)*(1 - t), 3 * t*(1 - t)*(1 - t), 3 * t*t*(1 - t), t*t*t };

				Geo::Vector p(0, 0, 0, 0);
				for(int k = 0; k < 4; k++) {
					for(int l = 0; l < 4; l++) {
						p += Bs[l] * Bt[k] * points[k * 4 + l];
					}
				}
				grid.setPoint(i, j, p);
			}
		}

		return grid;
	}
}