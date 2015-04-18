#ifndef OBJECT_RENDERABLE_OBJECT_HPP
#define OBJECT_RENDERABLE_OBJECT_HPP

#include "Object/BoundedObject.hpp"

namespace Object {
	class RenderableObject : public BoundedObject
	{
	public:
		virtual void render(const Render::Config &config) const = 0;
	};
}

#endif