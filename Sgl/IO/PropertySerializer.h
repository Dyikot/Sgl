#pragma once

#include <iostream>
#include <span>
#include <array>

namespace Sgl
{	
	class PropertySerializer
	{
	public:
		template <typename T>
		void Serialize(std::ostream& stream, const T& object, auto T::*... properties) const
		{
			((stream << object.*properties << '\t'), ...);
			stream << '\n';
		};

		template <typename T>
		void Serialize(std::ostream& stream, std::span<T> objects, auto T::*... properties) const
		{
			for(auto& object : objects)
			{
				((stream << object.*properties << '\t'), ...);
				stream << '\n';
			}
		};

		template <typename T>
		T Deserialize(std::istream& stream, auto T::*... properties) const
		{
			T object = {};			
			((stream >> object.*properties), ...);
			return object;
		};

		template <typename T, size_t Size>
		std::array<T, Size> Deserialize(std::istream& stream, auto T::*... properties) const
		{
			std::array<T, Size> objects;
			for(auto& object : objects)
			{
				((stream >> object.*properties), ...);
			}

			return objects;
		};
	};
}