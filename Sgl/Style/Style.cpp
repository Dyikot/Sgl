#include "Style.h"

namespace Sgl
{
	void StyleClass::AddTarget(StyleTarget styleTarget)
	{
		_selectors[styleTarget] = EmptySelector;
	}

	void StyleClass::ApplyStyleToElement()
	{
		_style = {};
		ApplyStyleToTarget(StyleTarget::Element);
	}

	void StyleClass::ApplyStyleTo(StyleTarget styleTarget)
	{
		ApplyStyleToElement();
		ApplyStyleToTarget(styleTarget);
	}

	void StyleClass::ApplyStyleToTarget(StyleTarget styleTarget)
	{
		auto selector = _selectors[styleTarget];

		if(selector)
		{
			selector(_style);
		}
	}
}
