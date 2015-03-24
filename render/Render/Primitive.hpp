#ifndef RENDER_PRIMITIVE_HPP
#define RENDER_PRIMITIVE_HPP

#include "Render/Object.hpp"
#include "Render/Grid.hpp"

#include <string>
#include <map>
#include <vector>

namespace Render {
	class Primitive : public Object
	{
	public:
		virtual Grid dice(const Config &config) const = 0;

		virtual void render(const Config &config) const;

		unsigned int newUniform(const std::string &name, unsigned int size);
		unsigned int uniformIndex(const std::string &name) const;
		float uniform(unsigned int index) const;
		void setUniform(unsigned int index, float value);

	private:
		std::map<std::string, int> mUniformIndices;
		std::vector<float> mUniforms;
	};
}

#endif