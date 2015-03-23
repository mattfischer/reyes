#include "Triangle.hpp"

#include <algorithm>

void Triangle::render(Framebuffer &framebuffer, const Vertex &p0, const Vertex &p1, const Vertex &p2, const Color &color)
{
	Geo::Vector pv0 = p0.position.project();
	Geo::Vector pv1 = p1.position.project();
	Geo::Vector pv2 = p2.position.project();

	float x0 = pv0.x();
	float y0 = pv0.y();
	float z0 = pv0.z();
	float w0 = p0.position.w();
	float x1 = pv1.x();
	float y1 = pv1.y();
	float z1 = pv1.z();
	float w1 = p1.position.w();
	float x2 = pv2.x();
	float y2 = pv2.y();
	float z2 = pv2.z();
	float w2 = p2.position.w();

	float det = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
	float winding = (det > 0) ? 1.0f : -1.0f;
	float invdet = 1.0f / det * winding;

	float x10 = (x1 - x0) * winding;
	float y10 = (y1 - y0) * winding;
	float x21 = (x2 - x1) * winding;
	float y21 = (y2 - y1) * winding;
	float x02 = (x0 - x2) * winding;
	float y02 = (y0 - y2) * winding;

	float xMin = std::min({ x0, x1, x2 });
	float xMax = std::max({ x0, x1, x2 });
	float yMin = std::min({ y0, y1, y2 });
	float yMax = std::max({ y0, y1, y2 });

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

	float iw0 = 1.0f / w0;
	float iw1 = 1.0f / w1;
	float iw2 = 1.0f / w2;

	Geo::Vector texCoord0 = p0.texCoord * iw0;
	Geo::Vector texCoord1 = p1.texCoord * iw1;
	Geo::Vector texCoord2 = p2.texCoord * iw2;

	Geo::Vector normal0 = p0.normal * iw0;
	Geo::Vector normal1 = p1.normal * iw1;
	Geo::Vector normal2 = p2.normal * iw2;

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
					unsigned short depth = unsigned short(z);
					if(depth <= framebuffer.getDepth(x, y, m)) {
						float iw = a * iw0 + b * iw1 + c * iw2;
						float w = 1.0f / iw;
						Geo::Vector texCoord = (a * texCoord0 + b * texCoord1 + c * texCoord2) * w;
						Geo::Vector normal = (a * normal0 + b * normal1 + c * normal2) * w;

						float s = texCoord.x();
						float t = texCoord.y();
						int si = int(std::floor(s));
						int ti = int(std::floor(t));
						float sf = s - float(si);
						float tf = t - float(ti);
						/*
						Color c0 = texture.data[ti * texture.width + si];
						Color cs = texture.data[ti * texture.width + si + 1];
						Color ct = texture.data[(ti + 1) * texture.width + si];
						Color cst = texture.data[(ti + 1) * texture.width + si + 1];
						Color c = c0 * sf * tf + cs * (1 - s) * t + ct * s * (1 - t) + cst * (1 - s) * (1 - t);
						*/
						float l = std::max(normal * (Geo::Vector(1, 1, -1, 0).normalize()), 0.0f);
						framebuffer.setPixel(x, y, m, color * l);
						framebuffer.setDepth(x, y, m, depth);
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
