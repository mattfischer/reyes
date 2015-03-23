#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "RenderObject.hpp"

#include <vector>
#include <memory>

class Container : public RenderObject
{
public:
	Container(std::vector<std::unique_ptr<RenderObject>> &&objects);

	virtual void render(const RenderConfig &config) const;

private:
	std::vector<std::unique_ptr<RenderObject>> mObjects;
};

#endif