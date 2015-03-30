#include "Render/Patch.hpp"

#include <algorithm>

namespace Render {
	Patch::Patch(Geo::Vector points[16], Texture &texture)
		: Primitive(texture)
	{
		mPoints = std::make_unique<Geo::Vector[]>(16);

		for(int i = 0; i < 16; i++) {
			mPoints[i] = points[i];
			boundingBox().surround(points[i]);
		}
	}

	Patch::Patch(Patch &&other)
		: Primitive(other), mPoints(std::move(other.mPoints))
	{
	}

	const Geo::Vector &Patch::point(int x, int y) const
	{
		return mPoints[y * 4 + x];
	}

	unsigned int Patch::numVaryingPoints() const
	{
		return 4;
	}

	void getDerivatives(const Geo::Vector points[], const Config &config, float s, float t, int &ds, int &dt)
	{
		float Bs[4] = { (1 - s)*(1 - s)*(1 - s), 3 * s*(1 - s)*(1 - s), 3 * s*s*(1 - s), s*s*s };
		float Bt[4] = { (1 - t)*(1 - t)*(1 - t), 3 * t*(1 - t)*(1 - t), 3 * t*t*(1 - t), t*t*t };
		float Bds[4] = { -3*(1 - s)*(1 - s), 3 * (1 - s)*(1 - s) - 3 * 2 * s * (1 - s), 3 * 2 * s*(1 - s) - 3 * s * s, 3 * s*s };
		float Bdt[4] = { -3 * (1 - t)*(1 - t), 3 * (1 - t)*(1 - t) - 3 * 2 * t * (1 - t), 3 * 2 * t*(1 - t) - 3 * t * t, 3 * t*t };

		Geo::Vector dsv(0, 0, 0, 0);
		Geo::Vector dtv(0, 0, 0, 0);
		Geo::Vector pv(0, 0, 0, 0);
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				dsv += Bds[i] * Bt[j] * points[i * 4 + j];
				dtv += Bs[i] * Bdt[j] * points[i * 4 + j];
				pv += Bs[i] * Bt[j] * points[i * 4 + j];
			}
		}

		Geo::Vector sv = pv + dsv;
		Geo::Vector tv = pv + dtv;
		sv = (config.viewport() * config.projection() * sv).project();
		tv = (config.viewport() * config.projection() * tv).project();
		pv = (config.viewport() * config.projection() * pv).project();
		dsv = sv - pv;
		dtv = tv - pv;
		dsv.setZ(0); dsv.setW(0);
		dtv.setZ(0); dtv.setW(0);

		ds = int(dsv.magnitude());
		dt = int(dtv.magnitude());
	}

	Grid Patch::dice(const Config &config) const
	{
		Geo::Matrix matrix = config.view() * transformation();
		Geo::Vector points[16];
		for(int k = 0; k < 4; k++) {
			for(int l = 0; l < 4; l++) {
				points[k * 4 + l] = matrix * point(l, k);
			}
		}

		int dsm = 0;
		int dtm = 0;
		int ds, dt;

		getDerivatives(points, config, 0.5f, 0.5f, ds, dt);
		dsm = std::max(ds, dsm);
		dtm = std::max(dt, dtm);

		getDerivatives(points, config, 0.0f, 0.0f, ds, dt);
		dsm = std::max(ds, dsm);
		dtm = std::max(dt, dtm);

		getDerivatives(points, config, 1.0f, 0.0f, ds, dt);
		dsm = std::max(ds, dsm);
		dtm = std::max(dt, dtm);

		getDerivatives(points, config, 0.0f, 1.0f, ds, dt);
		dsm = std::max(ds, dsm);
		dtm = std::max(dt, dtm);

		getDerivatives(points, config, 1.0f, 1.0f, ds, dt);
		dsm = std::max(ds, dsm);
		dtm = std::max(dt, dtm);

		Grid grid(dsm, dtm);

		for(int i = 0; i <= dsm; i++) {
			for(int j = 0; j <= dtm; j++) {
				float s = float(i) / float(dsm - 1);
				float t = float(j) / float(dtm - 1);

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