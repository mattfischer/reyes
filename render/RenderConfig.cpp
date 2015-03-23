#include "RenderConfig.hpp"

RenderConfig::RenderConfig(Framebuffer &framebuffer)
	: mFramebuffer(framebuffer)
{
}

Framebuffer &RenderConfig::framebuffer() const
{
	return mFramebuffer;
}

void RenderConfig::setView(const Geo::Matrix &view)
{
	mView = view;
}

const Geo::Matrix &RenderConfig::view() const
{
	return mView;
}

const Geo::Matrix &RenderConfig::projection() const
{
	return mProjection;
}

void RenderConfig::setProjection(const Geo::Matrix &projection)
{
	mProjection = projection;
}

const Geo::Matrix &RenderConfig::viewport() const
{
	return mViewport;
}

void RenderConfig::setViewport(const Geo::Matrix &viewport)
{
	mViewport = viewport;
}

RenderConfig::Type RenderConfig::type() const
{
	return mType;
}

void RenderConfig::setType(Type type)
{
	mType = type;
}