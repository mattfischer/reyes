#ifndef RENDER_OBJECT_HPP
#define RENDER_OBJECT_HPP

#include "Object.hpp"
#include "RenderConfig.hpp"

class RenderObject : public Object
{
public:
	virtual void render(const RenderConfig &config) const = 0;
};

#endif