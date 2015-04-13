#include "Render/Scene.hpp"

namespace Render {
	Scene::Scene(std::vector<std::unique_ptr<Object>> &&objects)
		: mObjects(std::move(objects))
	{
	}

	Scene::Scene(Scene &&other)
		: mObjects(std::move(other.mObjects))
	{
	}

	Scene &Scene::operator=(Scene &&other)
	{
		mObjects = std::move(other.mObjects);

		return *this;
	}

	void Scene::render(const Config &config) const
	{
		for(const std::unique_ptr<Object> &object : mObjects) {
			object->render(config);
		}
	}
}