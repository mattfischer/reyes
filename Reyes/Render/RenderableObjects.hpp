#ifndef RENDER_RENDERABLE_OBJECTS_HPP
#define RENDER_RENDERABLE_OBJECTS_HPP

#include "Render/RenderableObject.hpp"

#include <vector>
#include <memory>

namespace Render {
	class RenderableObjects : public RenderableObject
	{
	public:
		RenderableObjects(std::vector<std::unique_ptr<const RenderableObject>> &&objects);

		virtual void render(const Config &config) const;

	private:
		std::vector<std::unique_ptr<const RenderableObject>> mObjects;
	};
}

#endif