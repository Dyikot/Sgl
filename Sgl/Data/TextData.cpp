#include "TextData.h"
#include "../UI/UIElements/TextBlock.h"

namespace Sgl
{
	DataTemplate TextData::GetDefaultDataTemplate() const
	{
		return [](const Ref<IData>& data)
		{
			const auto& text = data.GetValueAs<TextData>().Text;
			auto textBlock = New<UIElements::TextBlock>();
			textBlock->SetText(text);
			return textBlock;
		};
	}
}