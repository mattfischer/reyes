#include "Render/Object.hpp"

namespace Render {
	const Geo::Matrix &Object::transformation() const
	{
		return mTransformation;
	}

	void Object::setTransformation(const Geo::Matrix &transformation)
	{
		mTransformation = transformation;
	}
}