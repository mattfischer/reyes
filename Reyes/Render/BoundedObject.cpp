#include "Render/BoundedObject.hpp"

#include "Render/Clipper.hpp"

namespace Render {
	const Geo::Box &BoundedObject::boundingBox() const
	{
		return mBox;
	}

	Geo::Box &BoundedObject::boundingBox()
	{
		return mBox;
	}

	bool BoundedObject::inFrustum(const Config &config) const
	{
		Geo::Matrix matrix = config.projection() * config.view() * transformation();
		return Clipper::boxInFrustum(boundingBox(), matrix);
	}
}