#include "Object/Camera.hpp"

namespace Object {
	Camera::Camera(const Geo::Matrix &projection)
		: mProjection(projection)
	{
	}

	const Geo::Matrix &Camera::projection() const
	{
		return mProjection;
	}
}