#include "Render/Scene.hpp"

namespace Render {
	Scene::Scene(std::vector<std::unique_ptr<RenderableObject>> &&objects, std::unique_ptr<Camera> camera)
		: mObjects(std::move(objects)), mCamera(std::move(camera))
	{
	}

	Scene::Scene(Scene &&other)
		: mObjects(std::move(other.mObjects)), mCamera(std::move(other.mCamera))
	{
	}

	Scene &Scene::operator=(Scene &&other)
	{
		mObjects = std::move(other.mObjects);
		mCamera = std::move(other.mCamera);

		return *this;
	}

	const Camera &Scene::camera() const
	{
		return *mCamera;
	}

	void Scene::render(const Config &config) const
	{
		for(const std::unique_ptr<RenderableObject> &object : mObjects) {
			object->render(config);
		}
	}
}