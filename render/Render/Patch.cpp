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

				Geo::Vector interp[16];
				for(int k = 0; k < 3; k++) {
					for(int l = 0; l < 3; l++) {
						interp[k * 4 + l] = points[k * 4 + l] * s * t + points[k * 4 + l + 1] * (1 - s) * t + points[(k + 1) * 4 + l] * s * (1 - t) + points[(k + 1) * 4 + l + 1] * (1 - s) * (1 - t);
					}
				}

				for(int k = 0; k < 2; k++) {
					for(int l = 0; l < 2; l++) {
						interp[k * 4 + l] = interp[k * 4 + l] * s * t + interp[k * 4 + l + 1] * (1 - s) * t + interp[(k + 1) * 4 + l] * s * (1 - t) + interp[(k + 1) * 4 + l + 1] * (1 - s) * (1 - t);
					}
				}

				Geo::Vector point = interp[0 * 4 + 0] * s * t + interp[0 * 4 + 1] * (1 - s) * t + interp[1 * 4 + 0] * s * (1 - t) + interp[1 * 4 + 1] * (1 - s) * (1 - t);
				grid.setPoint(i, j, point);
			}
		}

		return grid;
	}
}