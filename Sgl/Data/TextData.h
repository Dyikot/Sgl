#pragma once

#include <string>
#include "DataTemplate.h"

namespace Sgl
{
	class TextData : public IData
	{
	public:
		std::string Text;
	public:
		TextData(std::string text);
		TextData(const TextData&) = default;
		TextData(TextData&&) noexcept = default;

		DataTemplate GetDefaultDataTemplate() const override;
	};	
}