#ifndef OBJECT_RENDERABLE_OBJECTS_HPP
#define OBJECT_RENDERABLE_OBJECTS_HPP

#include "Object/RenderableObject.hpp"

#include <vector>
#include <memory>

namespace Object {
	class RenderableObjects : public RenderableObject
	{
	public:
		RenderableObjects(std::vector<std::unique_ptr<const RenderableObject>> &&objects);

		virtual void render(const Render::Config &config) const;

	private:
		std::vector<std::unique_ptr<const RenderableObject>> mObjects;
	};
}

#endif