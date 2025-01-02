#pragma once

#include "Control.h"
#include "../../Render/ContentVisualizer.h"

namespace Sgl
{
	class ControlContent: public Control
	{
	public:
		Any Content;
	private:
		std::unique_ptr<ContentVisualizer> _contentVisualizer;
	public:
		void OnRender(RenderContext& renderContext) override;

		template<typename T> requires std::derived_from<T, ContentVisualizer>
		void SetContentVisualizer()
		{
			_contentVisualizer = std::make_unique<T>(Content);
		}
	};
}