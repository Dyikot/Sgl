#include "Style.h"

namespace Sgl
{
	void StyleClass::AddTarget(StyleTarget styleTarget)
	{
		_selectors[styleTarget] = EmptySelector;
	}

	void StyleClass::ApplyStyleTo(StyleTarget styleTarget)
	{
		_style = {};
		SetStyleTo(StyleTarget::Element);

		if(styleTarget != StyleTarget::Element)
		{
			SetStyleTo(styleTarget);			
		}
	}

	void StyleClass::SetStyleTo(StyleTarget styleTarget)
	{
		auto selector = _selectors[styleTarget];

		if(selector)
		{
			selector(_style);
		}
	}
}
