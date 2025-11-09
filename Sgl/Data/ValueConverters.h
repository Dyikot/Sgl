#pragma once

#include <concepts>
#include <string>

namespace Sgl::Converters
{
	struct IntToStringConverter
	{
		std::string operator()(int value) const;
		int operator()(const std::string& value) const;
	};
}