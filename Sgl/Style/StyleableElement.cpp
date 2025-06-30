#include "StyleableElement.h"

namespace Sgl
{
	void StyleableElement::ApplyStyle()
	{
		for(auto& classStyle: Classes)
		{
			if(auto style = Styles.TryFind(classStyle); style != nullptr)
			{
				style->ApplyTo(*this);
			}
		}
	}
}