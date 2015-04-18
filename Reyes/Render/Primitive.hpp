#ifndef RENDER_PRIMITIVE_HPP
#define RENDER_PRIMITIVE_HPP

#include "Render/RenderableObject.hpp"
#include "Render/Grid.hpp"
#include "Render/Texture.hpp"
#include "Draw/Color.hpp"

#include <string>
#include <map>
#include <vector>

namespace Render {
	class Primitive : public RenderableObject
	{
	public:
		Primitive(Texture &texture);

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

	protected:
		struct Segment
		{
			float uMin;
			float vMin;
			float uMax;
			float vMax;

			Segment(float _uMin, float _vMin, float _uMax, float _vMax) : uMin(_uMin), vMin(_vMin), uMax(_uMax), vMax(_vMax) {}
		};

		virtual Grid dice(const Segment &segment, const Config &config) const = 0;
		virtual bool canDice(const Segment &segment, const Config &config) const = 0;

	private:
		virtual unsigned int numVaryingPoints() const = 0;

		void renderSegment(const Segment &segment, const Config &config, int depth) const;

		std::map<std::string, int> mUniformIndices;
		std::vector<float> mUniforms;

		std::map<std::string, int> mVaryingIndices;
		std::vector<std::vector<float>> mVaryings;

		Texture &mTexture;
	};
}

#endif