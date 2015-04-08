#include "Render/Rasterize.hpp"

#include <algorithm>

namespace Render{
	void Rasterize::renderQuad(Draw::Framebuffer &framebuffer, const Geo::Vector &p0, const Draw::Color &c0, Geo::Vector &p1, const Draw::Color &c1, Geo::Vector &p2, const Draw::Color &c2, Geo::Vector &p3, const Draw::Color &c3, bool draw123)
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
		float x3 = p3.x();
		float y3 = p3.y();
		float z3 = p3.z();

		float deta = (x1 - x0) * (y3 - y0) - (x3 - x0) * (y1 - y0);
		float detb = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
		float winding = (deta > 0) ? 1.0f : -1.0f;
		float invdeta = 1.0f / deta * winding;
		float invdetb = 1.0f / detb * winding;

		float x10 = (x1 - x0) * winding;
		float y10 = (y1 - y0) * winding;
		float x21 = (x2 - x1) * winding;
		float y21 = (y2 - y1) * winding;
		float x32 = (x3 - x2) * winding;
		float y32 = (y3 - y2) * winding;
		float x03 = (x0 - x3) * winding;
		float y03 = (y0 - y3) * winding;
		float x31 = (x3 - x1) * winding;
		float y31 = (y3 - y1) * winding;

		float xMin = std::max(std::min({ x0, x1, x2, x3 }), 0.0f);
		float xMax = std::min(std::max({ x0, x1, x2, x3 }), float(framebuffer.width() - 1));
		float yMin = std::max(std::min({ y0, y1, y2, y3 }), 0.0f);
		float yMax = std::min(std::max({ y0, y1, y2, y3 }), float(framebuffer.height() - 1));

		float multisampleBiasX[] = { -0.2f, 0.3f, -0.3f, 0.2f };
		float multisampleBiasY[] = { -0.3f, -0.2f, 0.2f, 0.3f };

		for(int y = int(yMin); y <= int(yMax); y++) {
			for(int x = int(xMin); x <= int(xMax); x++) {
				float xc = float(x) + 0.5f;
				float yc = float(y) + 0.5f;

				for(int m = 0; m < 4; m++) {
					float xs = xc + multisampleBiasX[m];
					float ys = yc + multisampleBiasY[m];

					float xs0 = xs - x0;
					float ys0 = ys - y0;
					float xs1 = xs - x1;
					float ys1 = ys - y1;
					float xs2 = xs - x2;
					float ys2 = ys - y2;
					float xs3 = xs - x3;
					float ys3 = ys - y3;

					float e0 = x31 * ys1 - y31 * xs1;
					float e1a = x03 * ys3 - y03 * xs3;
					float e1b = x32 * ys2 - y32 * xs2;
					float e3a = x10 * ys0 - y10 * xs0;
					float e3b = x21 * ys1 - y21 * xs1;

					float a;
					float b;
					float c;
					float z;
					Draw::Color color;

					if(e0 >= 0) {
						if(e1a >= 0 && e3a >= 0) {
							a = e0;
							b = e1a;
							c = e3a;

							z = (a * z0 + b * z1 + c * z3) * invdeta;
							color = (a * c0 + b * c1 + c * c3) * invdeta;
						} else {
							continue;
						}
					} else {
						if(draw123 && e1b >= 0 && e3b >= 0) {
							a = -e0;
							b = e1b;
							c = e3b;

							z = (a * z2 + b * z1 + c * z3) * invdetb;
							color = (a * c2 + b * c1 + c * c3) * invdetb;
						} else {
							continue;
						}
					}

					if(z <= framebuffer.getDepth(x, y, m)) {
						framebuffer.setPixel(x, y, m, color);
						framebuffer.setDepth(x, y, m, z);
					}
				}
			}
		}
	}
}