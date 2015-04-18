#ifndef RENDER_RENDERABLE_OBJECT_HPP
#define RENDER_RENDERABLE_OBJECT_HPP

#include "Render/BoundedObject.hpp"

namespace Render {
	class RenderableObject : public BoundedObject
	{
	public:
		virtual void render(const Config &config) const = 0;
	};
}

#endif