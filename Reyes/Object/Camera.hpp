#ifndef OBJECT_CAMERA_HPP
#define OBJECT_CAMERA_HPP

#include "Object/Object.hpp"

#include "Geo/Matrix.hpp"

namespace Object {
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