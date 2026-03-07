#pragma once

#include <string>

namespace Sgl::Converters
{
	struct IntToStringConverter
	{
		IntToStringConverter(int fallback = 0) : Fallback(fallback) {}

		int Fallback;

		std::string operator()(int value) const;
		int operator()(const std::string& value) const;
	};

	struct UIntToStringConverter
	{
		UIntToStringConverter(uint32_t fallback = 0) : Fallback(fallback) {}

		uint32_t Fallback;

		std::string operator()(uint32_t value) const;
		uint32_t operator()(const std::string& value) const;
	};

	struct Int64ToStringConverter
	{
		Int64ToStringConverter(int64_t fallback = 0) : Fallback(fallback) {}

		int64_t Fallback;

		std::string operator()(int64_t value) const;
		int64_t operator()(const std::string& value) const;
	};

	struct UInt64ToStringConverter
	{
		UInt64ToStringConverter(uint64_t fallback = 0) : Fallback(fallback) {}

		uint64_t Fallback;

		std::string operator()(uint64_t value) const;
		uint64_t operator()(const std::string& value) const;
	};

	struct FloatToStringConverter
	{
		FloatToStringConverter(float fallback = 0.0f) : Fallback(fallback) {}

		float Fallback;

		std::string operator()(float value) const;
		float operator()(const std::string& value) const;
	};

	struct DoubleToStringConverter
	{
		DoubleToStringConverter(double fallback = 0.0) : Fallback(fallback) {}

		double Fallback;

		std::string operator()(double value) const;
		double operator()(const std::string& value) const;
	};

	struct InverseBoolConverter
	{
		bool operator()(bool value) const;
	};
}