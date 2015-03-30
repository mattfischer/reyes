#include "Render/BoundedObject.hpp"

namespace Render {
	const Geo::Box &BoundedObject::boundingBox() const
	{
		return mBox;
	}

	Geo::Box &BoundedObject::boundingBox()
	{
		return mBox;
	}
}