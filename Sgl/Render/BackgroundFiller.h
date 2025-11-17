#pragma once

#include "RenderContext.h"

namespace Sgl
{
	class BackgroundFiller
	{
	private:
		RenderContext _context;
	public:
		explicit BackgroundFiller(RenderContext _context);

		void operator()(Color color);
		void operator()(const Texture& texture);
	};

	class RectBackgroundFiller
	{
	private:
		RenderContext _context;
		FRect _rect;
	public:
		RectBackgroundFiller(RenderContext _context, FRect rect);

		void operator()(Color color);
		void operator()(const Texture& texture);
	};
}