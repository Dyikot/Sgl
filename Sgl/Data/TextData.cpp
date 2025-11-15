#include "TextData.h"
#include "../UI/UIElements/TextBlock.h"

namespace Sgl
{
	TextData::TextData(std::string text):
		Text(std::move(text))
	{}

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