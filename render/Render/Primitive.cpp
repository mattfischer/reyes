#include "Render/Primitive.hpp"

#include <algorithm>

namespace Render {
	void Primitive::render(const Config &config) const
	{
		Grid grid = dice(config);

		unsigned int index = uniformIndex("color");
		Draw::Color color(unsigned char(uniform(index)), unsigned char(uniform(index + 1)), unsigned char(uniform(index + 2)));
		for(int x = 0; x < grid.width(); x++) {
			for(int y = 0; y < grid.height(); y++) {
				Geo::Vector u = grid.point(x + 1, y + 1) - grid.point(x, y);
				Geo::Vector v = grid.point(x, y + 1) - grid.point(x + 1, y);
				Geo::Vector normal = u % v;
				normal.setW(0);
				normal = normal.normalize();
				float l = std::max(normal * (Geo::Vector(1, 1, -1, 0).normalize()), 0.0f);
				grid.setColor(x, y, color * l);
			}
		}

		for(int x = 0; x <= grid.width(); x++) {
			for(int y = 0; y <= grid.height(); y++) {
				grid.setPoint(x, y, config.viewport() * (config.projection() * grid.point(x, y)).project());
			}
		}

		grid.render(config);
	}

	unsigned int Primitive::newUniform(const std::string &name, unsigned int size)
	{
		if(mUniformIndices.find(name) != mUniformIndices.end()) {
			return 0;
		}

		int index = mUniforms.size() + 1;
		mUniforms.resize(mUniforms.size() + size);
		mUniformIndices[name] = index;
		return index;
	}

	unsigned int Primitive::uniformIndex(const std::string &name) const
	{
		if(mUniformIndices.find(name) == mUniformIndices.end()) {
			return 0;
		}

		return mUniformIndices.find(name)->second;
	}

	void Primitive::setUniform(unsigned int index, float value)
	{
		if(index > 0 && index <= mUniforms.size()) {
			mUniforms[index - 1] = value;
		}
	}

	float Primitive::uniform(unsigned int index) const
	{
		if(index > 0 && index <= mUniforms.size()) {
			return mUniforms[index - 1];
		}

		return 0;
	}
}