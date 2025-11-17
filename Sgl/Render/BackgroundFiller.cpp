#include "BackgroundFiller.h"

namespace Sgl
{
	BackgroundFiller::BackgroundFiller(RenderContext _context):
		_context(_context)
	{}

	void BackgroundFiller::operator()(Color color)
	{
		_context.FillBackground(color);
	}

	void BackgroundFiller::operator()(const Texture& texture)
	{
		_context.DrawTexture(texture);
	}

	RectBackgroundFiller::RectBackgroundFiller(RenderContext _context, FRect rect):
		_context(_context),
		_rect(rect)
	{}

	void RectBackgroundFiller::operator()(Color color)
	{
		if(!color.IsTransparent())
		{
			_context.DrawRectangleFill(_rect, color);
		}
	}

	void RectBackgroundFiller::operator()(const Texture& texture)
	{
		_context.DrawTexture(texture, _rect);
	}
}

