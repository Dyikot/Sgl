#pragma once

#include <stdexcept>
#include <format>

namespace Sgl
{
	class Exception : public std::runtime_error
	{
	public:
		Exception(const std::string& message): std::runtime_error(message) {}
		Exception(const char* message) : std::runtime_error(message) {}

		template<typename... TArgs>
		Exception(std::format_string<TArgs...> message, TArgs&&... args):
			std::runtime_error(std::format(message, std::forward<TArgs>(args)...))
		{}
	};
}