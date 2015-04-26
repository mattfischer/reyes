#include "Object/Scene.hpp"

namespace Object {
	Scene::Scene(std::vector<std::unique_ptr<RenderableObject>> &&objects, std::unique_ptr<Camera> camera, std::vector<std::unique_ptr<Render::Texture>> &&textures)
		: mObjects(std::move(objects)), mCamera(std::move(camera)), mTextures(std::move(textures))
	{
	}

	Scene::Scene(Scene &&other)
		: mObjects(std::move(other.mObjects)), mCamera(std::move(other.mCamera)), mTextures(std::move(other.mTextures))
	{
	}

	Scene &Scene::operator=(Scene &&other)
	{
		mObjects = std::move(other.mObjects);
		mCamera = std::move(other.mCamera);
		mTextures = std::move(other.mTextures);

		return *this;
	}

	const Camera &Scene::camera() const
	{
		return *mCamera;
	}

	void Scene::render(const Render::Config &config) const
	{
		for(const std::unique_ptr<RenderableObject> &object : mObjects) {
			object->render(config);
		}
	}
}