#include "Object/BoundedObject.hpp"

#include "Render/Clipper.hpp"

namespace Object {
	const Geo::Box &BoundedObject::boundingBox() const
	{
		return mBox;
	}

	Geo::Box &BoundedObject::boundingBox()
	{
		return mBox;
	}

	bool BoundedObject::inFrustum(const Render::Config &config) const
	{
		Geo::Matrix matrix = config.projection() * config.view() * transformation();
		return Render::Clipper::boxInFrustum(boundingBox(), matrix);
	}
}