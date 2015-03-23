#ifndef RENDER_OBJECTS_HPP
#define RENDER_OBJECTS_HPP

#include "Render/Object.hpp"

#include <vector>
#include <memory>

namespace Render {
	class Objects : public Object
	{
	public:
		Objects(std::vector<std::unique_ptr<Object>> &&objects);

		virtual void render(const Config &config) const;

	private:
		std::vector<std::unique_ptr<Object>> mObjects;
	};
}

#endif