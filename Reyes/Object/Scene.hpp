#ifndef OBJECT_SCENE_HPP
#define OBJECT_SCENE_HPP

#include "Object/RenderableObject.hpp"
#include "Object/Camera.hpp"

#include "Render/Config.hpp"
#include "Render/Texture.hpp"

#include <vector>
#include <memory>

namespace Object {
	class Scene
	{
	public:
		Scene() = default;
		Scene(std::vector<std::unique_ptr<RenderableObject>> &&objects, std::unique_ptr<Camera> camera, std::vector<std::unique_ptr<Render::Texture>> &&textures);
		Scene(Scene &&other);

		Scene &operator=(Scene &&other);

		const Camera &camera() const;

		void render(const Render::Config &config) const;

	private:
		std::vector<std::unique_ptr<RenderableObject>> mObjects;
		std::unique_ptr<Camera> mCamera;
		std::vector<std::unique_ptr<Render::Texture>> mTextures;
	};
}

#endif