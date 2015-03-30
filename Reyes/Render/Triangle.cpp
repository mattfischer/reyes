#include "Render/Triangle.hpp"

#include <algorithm>

namespace Render{
	void Triangle::render(Draw::Framebuffer &framebuffer, const Geo::Vector &p0, const Draw::Color &c0, Geo::Vector &p1, const Draw::Color &c1, Geo::Vector &p2, const Draw::Color &c2)
	{
		float x0 = p0.x();
		float y0 = p0.y();
		float z0 = p0.z();
		float x1 = p1.x();
		float y1 = p1.y();
		float z1 = p1.z();
		float x2 = p2.x();
		float y2 = p2.y();
		float z2 = p2.z();

		float det = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
		float winding = (det > 0) ? 1.0f : -1.0f;
		float invdet = 1.0f / det * winding;

		float x10 = (x1 - x0) * winding;
		float y10 = (y1 - y0) * winding;
		float x21 = (x2 - x1) * winding;
		float y21 = (y2 - y1) * winding;
		float x02 = (x0 - x2) * winding;
		float y02 = (y0 - y2) * winding;

		float xMin = std::max(std::min({ x0, x1, x2 }), 0.0f);
		float xMax = std::min(std::max({ x0, x1, x2 }), float(framebuffer.width() - 1));
		float yMin = std::max(std::min({ y0, y1, y2 }), 0.0f);
		float yMax = std::min(std::max({ y0, y1, y2 }), float(framebuffer.height() - 1));

		float xs = std::floor(xMin) + 0.5f;
		float ys = std::floor(yMin) + 0.5f;
		float xs0 = xs - x0;
		float ys0 = ys - y0;
		float xs1 = xs - x1;
		float ys1 = ys - y1;
		float xs2 = xs - x2;
		float ys2 = ys - y2;

		float X0 = -y21;
		float X1 = -y02;
		float X2 = -y10;

		float Y0 = x21;
		float Y1 = x02;
		float Y2 = x10;

		float e0 = x21 * ys1 - y21 * xs1;
		float e1 = x02 * ys2 - y02 * xs2;
		float e2 = x10 * ys0 - y10 * xs0;

		float multisampleBiasX[] = { -0.2f, 0.3f, -0.3f, 0.2f };
		float multisampleBiasY[] = { -0.3f, -0.2f, 0.2f, 0.3f };

		for(int y = int(yMin); y <= int(yMax); y++) {
			float e0r = e0;
			float e1r = e1;
			float e2r = e2;

			for(int x = int(xMin); x <= int(xMax); x++) {
				for(int m = 0; m < 4; m++) {
					float e0m = e0 + X0 * multisampleBiasX[m] + Y0 * multisampleBiasY[m];
					float e1m = e1 + X1 * multisampleBiasX[m] + Y1 * multisampleBiasY[m];
					float e2m = e2 + X2 * multisampleBiasX[m] + Y2 * multisampleBiasY[m];

					if(e0m >= 0 && e1m >= 0 && e2m >= 0) {
						float a = e0m * invdet;
						float b = e1m * invdet;
						float c = e2m * invdet;

						float z = a * z0 + b * z1 + c * z2;
						if(z <= framebuffer.getDepth(x, y, m)) {
							Draw::Color color = a * c0 + b * c1 + c * c2;
							framebuffer.setPixel(x, y, m, color);
							framebuffer.setDepth(x, y, m, z);
						}
					}
				}

				e0 += X0;
				e1 += X1;
				e2 += X2;
			}

			e0 = e0r + Y0;
			e1 = e1r + Y1;
			e2 = e2r + Y2;
		}
	}
}