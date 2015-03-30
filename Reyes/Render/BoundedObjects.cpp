#include "Render/BoundedObjects.hpp"

namespace Render {
	BoundedObjects::BoundedObjects(std::vector<std::unique_ptr<const BoundedObject>> &&objects)
		: mObjects(std::move(objects))
	{
		for(const std::unique_ptr<const BoundedObject> &object : mObjects) {
			boundingBox().surround(object->boundingBox());
		}
	}

	void BoundedObjects::render(const Config &config) const
	{
		for(const std::unique_ptr<const BoundedObject> &object : mObjects) {
			object->render(config);
		}
	}
}