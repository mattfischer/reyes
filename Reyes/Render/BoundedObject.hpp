#ifndef RENDER_BOUNDED_OBJECT_HPP
#define RENDER_BOUNDED_OBJECT_HPP

#include "Render/Object.hpp"

#include "Geo/Box.hpp"

namespace Render {
	class BoundedObject : public Object
	{
	public:
		const Geo::Box &boundingBox() const;

	protected:
		Geo::Box &boundingBox();

	private:
		Geo::Box mBox;
	};
}

#endif