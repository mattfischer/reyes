#include "Object.hpp"

const Geo::Matrix &Object::transformation() const
{
	return mTransformation;
}

void Object::setTransformation(const Geo::Matrix &transformation)
{
	mTransformation = transformation;
}