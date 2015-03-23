#ifndef RENDER_CONFIG_HPP
#define RENDER_CONFIG_HPP

#include "Framebuffer.hpp"
#include "Geo/Matrix.hpp"

class RenderConfig
{
public:
	RenderConfig(Framebuffer &framebuffer);

	Framebuffer &framebuffer() const;

	const Geo::Matrix &view() const;
	void setView(const Geo::Matrix &view);

	const Geo::Matrix &projection() const;
	void setProjection(const Geo::Matrix &projection);

	const Geo::Matrix &viewport() const;
	void setViewport(const Geo::Matrix &viewport);

	enum class Type
	{
		Wireframe,
		Solid
	};

	Type type() const;
	void setType(Type type);

private:
	Framebuffer &mFramebuffer;
	Geo::Matrix mView;
	Geo::Matrix mProjection;
	Geo::Matrix mViewport;
	Type mType;
};

#endif