#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "Geo/Vector.hpp"
#include "Geo/Matrix.hpp"

class Object
{
public:
	const Geo::Matrix &transformation() const;
	void setTransformation(const Geo::Matrix &transformation);

private:
	Geo::Matrix mTransformation;
};

#endif