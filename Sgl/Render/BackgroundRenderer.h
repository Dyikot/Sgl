#pragma once

#include "RenderContext.h"
#include "TextureSource.h"

namespace Sgl
{
	class WindowBackgroundRenderer
	{
	private:
		RenderContext _context;
	public:
		explicit WindowBackgroundRenderer(RenderContext _context);

		void operator()(Color color);
		void operator()(const TextureSource& textureSource);
	};

	class UIElementBackgroundRenderer
	{
	private:
		RenderContext _context;
		FRect _rect;
	public:
		UIElementBackgroundRenderer(RenderContext _context, FRect rect);

		void operator()(Color color);
		void operator()(const TextureSource& textureSource);
	};
}