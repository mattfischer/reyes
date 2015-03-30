#ifndef RENDER_BOUNDED_OBJECTS_HPP
#define RENDER_BOUNDED_OBJECTS_HPP

#include "Render/BoundedObject.hpp"

#include <vector>
#include <memory>

namespace Render {
	class BoundedObjects : public BoundedObject
	{
	public:
		BoundedObjects(std::vector<std::unique_ptr<const BoundedObject>> &&objects);

		virtual void render(const Config &config) const;

	private:
		std::vector<std::unique_ptr<const BoundedObject>> mObjects;
	};
}

#endif