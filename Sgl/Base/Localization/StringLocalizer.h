#pragma once

#include <format>
#include "LocalizationStorage.h"

namespace Sgl::Localization
{
	class StringLocalizer
	{
	public:
		const std::string& operator()(const std::string& key) const
		{
			return GetLocalizationStorage()->GetLocalizedString(key);
		}

		template<typename... TArgs>
		std::string operator()(const std::string& key, TArgs&&... args) const
		{
			return std::vformat(operator()(key), std::make_format_args(args...));
		}
	};

	static inline StringLocalizer Localizer;
}