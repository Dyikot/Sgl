#pragma once

#include "IVisual.h"
#include "../Any.h"
#include <stdexcept>

namespace Sgl
{
	class ContentVisualizer: public IVisual
	{
	public:
		const Any& Content;
	public:
		ContentVisualizer(const Any& content):
			Content(content)
		{}
	};

	class StringVisualizer: public ContentVisualizer
	{
	public:
		StringVisualizer(const Any& content):
			ContentVisualizer(content)
		{
			if(!content.Is<std::string>())
			{
				throw std::invalid_argument("Content type is not std::string!\n");
			}
		}

		void OnRender(RenderContext& renderContext) override
		{

		}
	};
}