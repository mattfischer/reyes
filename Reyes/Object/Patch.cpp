#include "Object/Patch.hpp"

#include <algorithm>

namespace Object {
	Patch::Patch(Geo::Vector points[16], Render::Texture &texture)
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

	void getDerivatives(const Geo::Vector points[], const Render::Config &config, float s, float t, int &ds, int &dt, int &a)
	{
		float Bs[4] = { (1 - s)*(1 - s)*(1 - s), 3 * s*(1 - s)*(1 - s), 3 * s*s*(1 - s), s*s*s };
		float Bt[4] = { (1 - t)*(1 - t)*(1 - t), 3 * t*(1 - t)*(1 - t), 3 * t*t*(1 - t), t*t*t };
		float Bds[4] = { -3 * (1 - s)*(1 - s), 3 * (1 - s)*(1 - s) - 3 * 2 * s * (1 - s), 3 * 2 * s*(1 - s) - 3 * s * s, 3 * s*s };
		float Bdt[4] = { -3 * (1 - t)*(1 - t), 3 * (1 - t)*(1 - t) - 3 * 2 * t * (1 - t), 3 * 2 * t*(1 - t) - 3 * t * t, 3 * t*t };

		Geo::Vector dsv(0, 0, 0, 0);
		Geo::Vector dtv(0, 0, 0, 0);
		Geo::Vector pv(0, 0, 0, 0);
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) {
				dsv += (Bds[j] * Bt[i]) * points[i * 4 + j];
				dtv += (Bs[j] * Bdt[i]) * points[i * 4 + j];
				pv += (Bs[j] * Bt[i]) * points[i * 4 + j];
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
		a = int(std::abs((dsv % dtv).z()));
	}

	bool Patch::canDice(const Segment &segment, const Render::Config &config) const
	{
		Geo::Matrix matrix = config.view() * transformation();
		Geo::Vector points[16];
		for(int k = 0; k < 4; k++) {
			for(int l = 0; l < 4; l++) {
				points[k * 4 + l] = matrix * point(l, k);
			}
		}

		float pointsU[] = { segment.uMin, segment.uMax, segment.uMin, segment.uMax };
		float pointsV[] = { segment.vMin, segment.vMin, segment.vMax, segment.vMax };
		for(int i = 0; i < 4; i++) {
			int ds;
			int dt;
			int a;

			getDerivatives(points, config, pointsU[i], pointsV[i], ds, dt, a);

			a = std::abs(int(a * (segment.uMax - segment.uMin) * (segment.vMax - segment.vMin)));
			if(a > 256) {
				return false;
			}
		}

		return true;
	}

	Render::Grid Patch::dice(const Segment &segment, const Render::Config &config) const
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
		int a;
		getDerivatives(points, config, (segment.uMin + segment.uMax) / 2, (segment.vMin + segment.vMax) / 2, dsm, dtm, a);

		dsm = std::max(int(dsm * (segment.uMax - segment.uMin)), 4);
		dtm = std::max(int(dtm * (segment.vMax - segment.vMin)), 4);
		Render::Grid grid(dsm, dtm);

		for(int i = 0; i <= dsm; i++) {
			for(int j = 0; j <= dtm; j++) {
				float s = segment.uMin + (segment.uMax - segment.uMin) * float(i) / float(dsm - 1);
				float t = segment.vMin + (segment.vMax - segment.vMin) * float(j) / float(dtm - 1);

				float Bs[4] = { (1 - s)*(1 - s)*(1 - s), 3 * s*(1 - s)*(1 - s), 3 * s*s*(1 - s), s*s*s };
				float Bt[4] = { (1 - t)*(1 - t)*(1 - t), 3 * t*(1 - t)*(1 - t), 3 * t*t*(1 - t), t*t*t };

				Geo::Vector p(0, 0, 0, 0);
				for(int k = 0; k < 4; k++) {
					for(int l = 0; l < 4; l++) {
						p += (Bs[l] * Bt[k]) * points[k * 4 + l];
					}
				}
				grid.setPoint(i, j, p);
			}
		}

		return grid;
	}
}