#include "Render/Config.hpp"

namespace Render {
	Config::Config(Draw::Framebuffer &framebuffer)
		: mFramebuffer(framebuffer)
	{
	}

	Draw::Framebuffer &Config::framebuffer() const
	{
		return mFramebuffer;
	}

	void Config::setView(const Geo::Matrix &view)
	{
		mView = view;
	}

	const Geo::Matrix &Config::view() const
	{
		return mView;
	}

	const Geo::Matrix &Config::projection() const
	{
		return mProjection;
	}

	void Config::setProjection(const Geo::Matrix &projection)
	{
		mProjection = projection;
	}

	const Geo::Matrix &Config::viewport() const
	{
		return mViewport;
	}

	void Config::setViewport(const Geo::Matrix &viewport)
	{
		mViewport = viewport;
	}

	Config::Type Config::type() const
	{
		return mType;
	}

	void Config::setType(Type type)
	{
		mType = type;
	}
}