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

	struct LongToStringConverter
	{
		std::string operator()(long value) const;
		long operator()(const std::string& value) const;
	};

	struct LongLongToStringConverter
	{
		std::string operator()(long long value) const;
		long long operator()(const std::string& value) const;
	};

	struct UnsignedIntToStringConverter
	{
		std::string operator()(unsigned int value) const;
		unsigned int operator()(const std::string& value) const;
	};

	struct UnsignedLongToStringConverter
	{
		std::string operator()(unsigned long value) const;
		unsigned long operator()(const std::string& value) const;
	};

	struct UnsignedLongLongToStringConverter
	{
		std::string operator()(unsigned long long value) const;
		unsigned long long operator()(const std::string& value) const;
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