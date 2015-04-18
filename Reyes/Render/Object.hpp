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
		void transform(const Geo::Matrix &transformation);

	private:
		Geo::Matrix mTransformation;
	};
}

#endif