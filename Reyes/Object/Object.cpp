#include "Object/Object.hpp"

namespace Object {
	const Geo::Matrix &Object::transformation() const
	{
		return mTransformation;
	}

	void Object::transform(const Geo::Matrix &transformation)
	{
		mTransformation = transformation * mTransformation;
	}
}