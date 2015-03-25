#ifndef RENDER_OBJECT_HPP
#define RENDER_OBJECT_HPP

#include "Geo/Vector.hpp"
#include "Geo/Matrix.hpp"

#include "Render/Config.hpp"

namespace Render {
	class Object
	{
	public:
		const Geo::Matrix &transformation() const;
		void setTransformation(const Geo::Matrix &transformation);

		virtual void render(const Config &config) const = 0;

	private:
		Geo::Matrix mTransformation;
	};
}

#endif