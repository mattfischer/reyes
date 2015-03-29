#ifndef RENDER_PRIMITIVE_HPP
#define RENDER_PRIMITIVE_HPP

#include "Render/Object.hpp"
#include "Render/Grid.hpp"
#include "Render/Texture.hpp"
#include "Draw/Color.hpp"

#include <string>
#include <map>
#include <vector>

namespace Render {
	class Primitive : public Object
	{
	public:
		Primitive(Texture &texture);

		virtual Grid dice(const Config &config) const = 0;

		virtual void render(const Config &config) const;

		unsigned int newUniform(const std::string &name, unsigned int size);
		unsigned int uniformIndex(const std::string &name) const;

		float uniform(unsigned int index) const;
		Draw::Color uniformColor(unsigned int index) const;
		Geo::Vector uniformVector(unsigned int index) const;

		void setUniform(unsigned int index, float value);
		void setUniformColor(unsigned int index, const Draw::Color &color);
		void setUniformVector(unsigned int index, const Geo::Vector &vector);

		unsigned int newVarying(const std::string &name, unsigned int size);
		unsigned int varyingIndex(const std::string &name) const;

		float varying(unsigned int index, unsigned int point) const;
		Draw::Color varyingColor(unsigned int index, unsigned int point) const;
		Geo::Vector varyingVector(unsigned int index, unsigned int point) const;

		void setVarying(unsigned int index, unsigned int point, float value);
		void setVaryingColor(unsigned int index, unsigned int point, const Draw::Color &color);
		void setVaryingVector(unsigned int index, unsigned int point, const Geo::Vector &vector);

	private:
		virtual unsigned int numVaryingPoints() const = 0;

		std::map<std::string, int> mUniformIndices;
		std::vector<float> mUniforms;

		std::map<std::string, int> mVaryingIndices;
		std::vector<std::vector<float>> mVaryings;

		Texture &mTexture;
	};
}

#endif