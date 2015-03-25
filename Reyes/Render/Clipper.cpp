#include "Render/Clipper.hpp"

namespace Render {
	static Geo::Vector clipPlanes[] = {
			{ 1, 0, 0, 1 },
			{ -1, 0, 0, 1 },
			{ 0, 1, 0, 1 },
			{ 0, -1, 0, 1 },
			{ 0, 0, 1, 1 },
			{ 0, 0, -1, 1 }
	};

	static bool clipLineToPlane(Geo::Vector &a, Geo::Vector &b, const Geo::Vector &normal)
	{
		float aN = a * normal;
		float bN = b * normal;

		if(aN >= 0 && bN >= 0) {
			return true;
		}

		if(aN <= 0 && bN <= 0) {
			return false;
		}

		Geo::Vector m = b - a;
		float mN = m * normal;
		float t = -aN / mN;
		Geo::Vector i = a + t * m;

		if(aN >= 0) {
			b = i;
		} else {
			a = i;
		}

		return true;
	}

	bool Clipper::clipLine(Geo::Vector &a, Geo::Vector &b)
	{
		for(int i = 0; i < 6; i++) {
			if(!clipLineToPlane(a, b, clipPlanes[i])) return false;
		}

		return true;
	}

	static bool clipPolygonToPlane(Clipper::Polygon &polygon, const Geo::Vector &normal)
	{
		int o = 0;
		Clipper::Vertex a = polygon.vertices[polygon.numVertices - 1];
		float aN = a.position * normal;
		Clipper::Vertex b = polygon.vertices[0];
		for(int i = 0; i < polygon.numVertices; i++) {
			Clipper::Vertex next = polygon.vertices[i + 1];
			float bN = b.position * normal;
			if(bN >= 0) {
				if(aN < 0) {
					Geo::Vector m = b.position - a.position;
					Geo::Vector mt = b.texCoord - a.texCoord;
					Geo::Vector mn = b.normal - a.normal;
					float mN = m * normal;
					float t = -aN / mN;
					Geo::Vector i = a.position + t * m;
					Geo::Vector it = a.texCoord + t * mt;
					Geo::Vector in = a.normal + t * mn;
					polygon.vertices[o] = Clipper::Vertex(i, it, in);
					o++;
				}
				polygon.vertices[o] = b;
				o++;
			} else {
				if(aN > 0) {
					Geo::Vector m = b.position - a.position;
					Geo::Vector mt = b.texCoord - a.texCoord;
					Geo::Vector mn = b.normal - a.normal;
					float mN = m * normal;
					float t = -aN / mN;
					Geo::Vector i = a.position + t * m;
					Geo::Vector it = a.texCoord + t * mt;
					Geo::Vector in = a.normal + t * mn;
					polygon.vertices[o] = Clipper::Vertex(i, it, in);
					o++;
				}
			}
			a = b;
			aN = bN;
			b = next;
		}

		polygon.numVertices = o;
		return polygon.numVertices > 0;
	}

	bool Clipper::clipPolygon(Polygon &polygon)
	{
		for(int i = 0; i < 6; i++) {
			if(!clipPolygonToPlane(polygon, clipPlanes[i])) return false;
		}

		return true;
	}
}