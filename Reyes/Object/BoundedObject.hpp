#ifndef OBJECT_BOUNDED_OBJECT_HPP
#define OBJECT_BOUNDED_OBJECT_HPP

#include "Object/Object.hpp"
#include "Render/Config.hpp"

#include "Geo/Box.hpp"

namespace Object {
	class BoundedObject : public Object
	{
	public:
		const Geo::Box &boundingBox() const;

		bool inFrustum(const Render::Config &config) const;

	protected:
		Geo::Box &boundingBox();

	private:
		Geo::Box mBox;
	};
}

#endif