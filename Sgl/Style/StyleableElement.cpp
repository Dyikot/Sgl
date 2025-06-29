#include "StyleableElement.h"

namespace Sgl
{
	void StyleableElement::ApplyStyle()
	{
		for(auto& classStyle: Classes)
		{
			if(auto it = Styles.find(classStyle); it != Styles.end())
			{
				it->second->ApplyTo(*this);
			}
		}
	}
}