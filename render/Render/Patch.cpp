#include "Render/Patch.hpp"

namespace Render {
	Patch::Patch(Geo::Vector points[16], const Draw::Color &color)
		: mColor(color)
	{
		mPoints = std::make_unique<Geo::Vector[]>(16);

		for(int i = 0; i < 16; i++) {
			mPoints[i] = points[i];
		}
	}

	Patch::Patch(Patch &&other)
		: mPoints(std::move(other.mPoints)), mColor(other.mColor)
	{
	}

	const Geo::Vector &Patch::point(int x, int y) const
	{
		return mPoints[y * 4 + x];
	}

	Grid Patch::tesselate(const Config &config, int divisions) const
	{
		Grid grid(divisions + 1, divisions + 1, mColor);

		for(int i = 0; i <= divisions; i++) {
			for(int j = 0; j <= divisions; j++) {
				float s = float(i) / float(divisions);
				float t = float(j) / float(divisions);

				Geo::Vector interp[16];

				for(int k = 0; k < 4; k++) {
					for(int l = 0; l < 4; l++) {
						interp[k * 4 + l] = config.projection() * config.view() * transformation() * point(l, k);
					}
				}

				for(int k = 0; k < 3; k++) {
					for(int l = 0; l < 3; l++) {
						interp[k * 4 + l] = interp[k * 4 + l] * s * t + interp[k * 4 + l + 1] * (1 - s) * t + interp[(k + 1) * 4 + l] * s * (1 - t) + interp[(k + 1) * 4 + l + 1] * (1 - s) * (1 - t);
					}
				}

				for(int k = 0; k < 2; k++) {
					for(int l = 0; l < 2; l++) {
						interp[k * 4 + l] = interp[k * 4 + l] * s * t + interp[k * 4 + l + 1] * (1 - s) * t + interp[(k + 1) * 4 + l] * s * (1 - t) + interp[(k + 1) * 4 + l + 1] * (1 - s) * (1 - t);
					}
				}

				Geo::Vector point = interp[0 * 4 + 0] * s * t + interp[0 * 4 + 1] * (1 - s) * t + interp[1 * 4 + 0] * s * (1 - t) + interp[1 * 4 + 1] * (1 - s) * (1 - t);
				Geo::Vector u = (interp[1 * 4 + 0] - interp[0 * 4 + 0]) * s + (interp[1 * 4 + 1] - interp[0 * 4 + 1]) * (1 - s);
				Geo::Vector v = (interp[0 * 4 + 1] - interp[0 * 4 + 0]) * t + (interp[1 * 4 + 1] - interp[1 * 4 + 0]) * (1 - t);
				Geo::Vector normal = v % u;
				normal.setW(0);
				grid.setPoint(i, j, Grid::Point(point, Geo::Vector(s, t, 0), normal.normalize()));
			}
		}

		return grid;
	}

	void Patch::render(const Config &config) const
	{
		Grid grid = tesselate(config, 16);

		grid.render(config);
	}
}