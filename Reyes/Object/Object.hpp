#ifndef OBJECT_OBJECT_HPP
#define OBJECT_OBJECT_HPP

#include "Geo/Vector.hpp"
#include "Geo/Matrix.hpp"

#include "Render/Config.hpp"

namespace Object {
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