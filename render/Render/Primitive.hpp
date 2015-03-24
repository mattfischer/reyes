#ifndef RENDER_PRIMITIVE_HPP
#define RENDER_PRIMITIVE_HPP

#include "Render/Object.hpp"
#include "Render/Grid.hpp"

namespace Render {
	class Primitive : public Object
	{
	public:
		virtual Grid dice(const Config &config) const = 0;

		virtual void render(const Config &config) const;
	};
}

#endif