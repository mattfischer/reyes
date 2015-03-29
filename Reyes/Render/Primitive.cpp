#include "Render/Primitive.hpp"

#include <algorithm>

namespace Render {
	Primitive::Primitive(Texture &texture)
		: mTexture(texture)
	{
	}

	void Primitive::render(const Config &config) const
	{
		Grid grid = dice(config);

		std::vector<float> varyings;
		varyings.resize(mVaryings.size());

		Geo::Vector light = Geo::Vector(1, 1, -1, 0).normalize();
		for(int x = 0; x < grid.width(); x++) {
			for(int y = 0; y < grid.height(); y++) {
				Geo::Vector u = grid.point(x + 1, y) - grid.point(x, y);
				Geo::Vector v = grid.point(x, y + 1) - grid.point(x, y);
				Geo::Vector normal = u % v;
				normal = normal.normalize();
				float l = std::max(normal * light, 0.0f);

				float s = float(x) / float(grid.width());
				float t = float(y) / float(grid.height());
				for(unsigned int i = 0; i < varyings.size(); i++) {
					varyings[i] = varying(i + 1, 0) * (1 - s) * (1 - t) + varying(i + 1, 1) * s * (1 - t) + varying(i + 1, 2) * (1 - s) * t + varying(i + 1, 3) * s * t;
				}
				Draw::Color color = mTexture.sample(varyings[0], varyings[1]);
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

	unsigned int Primitive::newVarying(const std::string &name, unsigned int size)
	{
		if(mVaryingIndices.find(name) != mVaryingIndices.end()) {
			return 0;
		}

		int index = mVaryings.size() + 1;
		mVaryings.resize(mVaryings.size() + size);
		for(unsigned int i = 0; i < size; i++) {
			mVaryings[index + i - 1].resize(numVaryingPoints());
		}
		mVaryingIndices[name] = index;
		return index;
	}

	unsigned int Primitive::varyingIndex(const std::string &name) const
	{
		if(mVaryingIndices.find(name) == mVaryingIndices.end()) {
			return 0;
		}

		return mVaryingIndices.find(name)->second;
	}

	float Primitive::varying(unsigned int index, unsigned int point) const
	{
		if(index > 0 && index <= mVaryings.size() && point >=0 && point < numVaryingPoints()) {
			return mVaryings[index - 1][point];
		}

		return 0;
	}

	Draw::Color Primitive::varyingColor(unsigned int index, unsigned int point) const
	{
		return Draw::Color(varying(index, point), varying(index + 1, point), varying(index + 2, point));
	}

	Geo::Vector Primitive::varyingVector(unsigned int index, unsigned int point) const
	{
		return Geo::Vector(varying(index, point), varying(index + 1, point), varying(index + 2, point));
	}

	void Primitive::setVarying(unsigned int index, unsigned int point, float value)
	{
		if(index > 0 && index <= mVaryings.size() && point >= 0 && point < numVaryingPoints()) {
			mVaryings[index - 1][point] = value;
		}
	}

	void Primitive::setVaryingColor(unsigned int index, unsigned int point, const Draw::Color &color)
	{
		setVarying(index + 0, point, color.r);
		setVarying(index + 1, point, color.g);
		setVarying(index + 2, point, color.b);
	}

	void Primitive::setVaryingVector(unsigned int index, unsigned int point, const Geo::Vector &vector)
	{
		setVarying(index + 0, point, vector.x());
		setVarying(index + 1, point, vector.y());
		setVarying(index + 2, point, vector.z());
	}
}