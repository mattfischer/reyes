#include "Render/Camera.hpp"

namespace Render {
	Camera::Camera(const Geo::Matrix &projection)
		: mProjection(projection)
	{
	}

	const Geo::Matrix &Camera::projection() const
	{
		return mProjection;
	}
}