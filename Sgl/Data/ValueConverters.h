#pragma once

#include <concepts>
#include <string>

namespace Sgl::Converters
{
	struct Int32ToStringConverter
	{
		std::string operator()(int value) const;
		int operator()(const std::string& value) const;
	};

	struct UInt32ToStringConverter
	{
		std::string operator()(uint32_t value) const;
		uint32_t operator()(const std::string& value) const;
	};

	struct Int64ToStringConverter
	{
		std::string operator()(int64_t value) const;
		int64_t operator()(const std::string& value) const;
	};

	struct UInt64ToStringConverter
	{
		std::string operator()(uint64_t value) const;
		uint64_t operator()(const std::string& value) const;
	};

	struct FloatToStringConverter
	{
		std::string operator()(float value) const;
		float operator()(const std::string& value) const;
	};

	struct DoubleToStringConverter
	{
		std::string operator()(double value) const;
		double operator()(const std::string& value) const;
	};
}