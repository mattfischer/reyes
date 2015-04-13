#ifndef RENDER_SCENE_HPP
#define RENDER_SCENE_HPP

#include "Render/Object.hpp"
#include "Render/Config.hpp"

#include <vector>
#include <memory>

namespace Render {
	class Scene
	{
	public:
		Scene() = default;
		Scene(std::vector<std::unique_ptr<Object>> &&objects);
		Scene(Scene &&other);

		Scene &operator=(Scene &&other);

		void render(const Config &config) const;

	private:
		std::vector<std::unique_ptr<Object>> mObjects;
	};
}

#endif