#pragma once

#include "../Base/Delegate.h"
#include "RenderContext.h"

namespace Sgl
{
	using RenderFragment = Action<RenderContext, const FRect&>;
}

namespace Sgl::RenderFragments
{
	struct Rectangle
	{
		Color Background;

		void operator()(RenderContext context, const FRect& rect) const
		{
			context.DrawRectangleFill(rect, Background);
		}
	};

	struct RoundedRectangle
	{
		Color Background;
		float CornersRadius;

		void operator()(RenderContext context, const FRect& rect) const
		{
			context.DrawRectangleFill(rect, CornersRadius, Background);
		}
	};

	struct Image
	{
		Texture Background;

		void operator()(RenderContext context, const FRect& rect) const
		{
			context.DrawTexture(Background, &rect, nullptr);
		}
	};

	struct RoundedImage
	{
		Texture Background;
		float CornersRadius;

		void operator()(RenderContext context, const FRect& rect) const
		{
			context.DrawRectangleFill(rect, CornersRadius, Background);
		}
	};
}