#include "Container.hpp"

Container::Container(std::vector<std::unique_ptr<RenderObject>> &&objects)
	: mObjects(std::move(objects))
{
}

void Container::render(const RenderConfig &config) const
{
	for(auto &object : mObjects) {
		object->render(config);
	}
}