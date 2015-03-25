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
		Geo::Vector corners[4] = { point(0, 0), point(0, 3), point(3, 0), point(3, 3) };
		for(int i = 0; i < 4; i++) {
			corners[i] = (config.viewport() * config.projection() * corners[i]).project();
		}

		Geo::Matrix matrix = config.view() * transformation();
		Geo::Vector points[16];
		for(int k = 0; k < 4; k++) {
			for(int l = 0; l < 4; l++) {
				points[k * 4 + l] = matrix * point(l, k);
			}
		}

		float b[4] = { 0.125f, 0.375f, 0.375f, 0.125f };
		float d[4] = { -0.75f, -0.75f, 0.75f, 0.75f };

		Geo::Vector du(0, 0, 0, 0);
		Geo::Vector dv(0, 0, 0, 0);
		Geo::Vector p(0, 0, 0, 0);
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				du += b[i] * d[j] * points[i * 4 + j];
				dv += d[i] * b[j] * points[i * 4 + j];
				p += b[i] * b[j] * points[i * 4 + j];
			}
		}
		Geo::Vector u = p + du;
		Geo::Vector v = p + dv;
		u = (config.viewport() * config.projection() * u).project();
		v = (config.viewport() * config.projection() * v).project();
		p = (config.viewport() * config.projection() * p).project();
		du = u - p;
		dv = v - p;

		int um = int(du.magnitude());
		int vm = int(dv.magnitude());
		Grid grid(um, vm);

		for(int i = 0; i <= um; i++) {
			for(int j = 0; j <= vm; j++) {
				float s = float(i) / float(um);
				float t = float(j) / float(vm);

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