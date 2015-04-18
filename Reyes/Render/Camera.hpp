#ifndef RENDER_CAMERA_HPP
#define RENDER_CAMERA_HPP

#include "Render/Object.hpp"

#include "Geo/Matrix.hpp"

namespace Render {
	class Camera : public Object
	{
	public:
		Camera(const Geo::Matrix &projection);

		const Geo::Matrix &projection() const;

	private:
		Geo::Matrix mProjection;
	};
}

#endif