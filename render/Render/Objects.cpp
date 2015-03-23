#include "Render/Objects.hpp"

namespace Render {
	Objects::Objects(std::vector<std::unique_ptr<Object>> &&objects)
		: mObjects(std::move(objects))
	{
	}

	void Objects::render(const Config &config) const
	{
		for(auto &object : mObjects) {
			object->render(config);
		}
	}
}