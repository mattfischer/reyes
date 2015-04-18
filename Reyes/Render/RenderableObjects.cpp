#include "Render/RenderableObjects.hpp"

namespace Render {
	RenderableObjects::RenderableObjects(std::vector<std::unique_ptr<const RenderableObject>> &&objects)
		: mObjects(std::move(objects))
	{
		for(const std::unique_ptr<const RenderableObject> &object : mObjects) {
			boundingBox().surround(object->boundingBox());
		}
	}

	void RenderableObjects::render(const Config &config) const
	{
		if(!inFrustum(config)) {
			return;
		}

		for(const std::unique_ptr<const RenderableObject> &object : mObjects) {
			object->render(config);
		}
	}
}