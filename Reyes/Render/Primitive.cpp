#include "Render/Primitive.hpp"

#include <algorithm>

namespace Render {
	void Primitive::render(const Config &config) const
	{
		Grid grid = dice(config);

		unsigned int index = uniformIndex("color");
		Draw::Color color = uniformColor(index);
		Geo::Vector light = Geo::Vector(1, 1, -1, 0).normalize();
		for(int x = 0; x < grid.width(); x++) {
			for(int y = 0; y < grid.height(); y++) {
				Geo::Vector u = grid.point(x + 1, y) - grid.point(x, y);
				Geo::Vector v = grid.point(x, y + 1) - grid.point(x, y);
				Geo::Vector normal = u % v;
				normal = normal.normalize();
				float l = std::max(normal * light, 0.0f);
				grid.setColor(x, y, color * l);
			}
		}

		Geo::Matrix matrix = config.viewport() * config.projection();
		for(int x = 0; x <= grid.width(); x++) {
			for(int y = 0; y <= grid.height(); y++) {
				grid.setPoint(x, y, (matrix * grid.point(x, y)).project());
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

	float Primitive::uniform(unsigned int index) const
	{
		if(index > 0 && index <= mUniforms.size()) {
			return mUniforms[index - 1];
		}

		return 0;
	}

	Draw::Color Primitive::uniformColor(unsigned int index) const
	{
		return Draw::Color(uniform(index), uniform(index + 1), uniform(index + 2));
	}

	Geo::Vector Primitive::uniformVector(unsigned int index) const
	{
		return Geo::Vector(uniform(index), uniform(index + 1), uniform(index + 2));
	}

	void Primitive::setUniform(unsigned int index, float value)
	{
		if(index > 0 && index <= mUniforms.size()) {
			mUniforms[index - 1] = value;
		}
	}

	void Primitive::setUniformColor(unsigned int index, const Draw::Color &color)
	{
		setUniform(index + 0, color.r);
		setUniform(index + 1, color.g);
		setUniform(index + 2, color.b);
	}

	void Primitive::setUniformVector(unsigned int index, const Geo::Vector &vector)
	{
		setUniform(index + 0, vector.x());
		setUniform(index + 1, vector.y());
		setUniform(index + 2, vector.z());
	}
}