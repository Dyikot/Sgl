#include "ContentControl.h"

namespace Sgl
{
	void ControlContent::OnRender(RenderContext& renderContext)
	{
		Control::OnRender(renderContext);

		if(_contentVisualizer)
		{
			_contentVisualizer->OnRender(renderContext);
		}
	}
}

