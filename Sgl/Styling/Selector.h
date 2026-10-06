#pragma once

#include <vector>
#include <string>

#include "../Base/Ref.h"

namespace Sgl
{
	class Styleable;

	class Selector
	{
	public:
		using TypeComparer = bool(*)(const Styleable&);
	public:
		Selector();
		Selector(const Selector& other);
		Selector(Selector&& other) noexcept;
		~Selector();

		template<typename T>
		Selector& OfType()
		{
			_data->typeComparer = CompareTypeById<T>;
			return *this;
		}

		template<typename T>
		Selector& Is()
		{
			_data->typeComparer = CompareType<T>;
			return *this;
		}

		Selector& Name(std::string name);
		Selector& Class(std::string className);

		bool Match(const Styleable& element) const;

		Selector& operator=(const Selector& other);
		Selector& operator=(Selector&& other) noexcept;

	private:
		template<typename T>
		static bool CompareType(const Styleable& element)
		{
			return dynamic_cast<const T*>(&element);
		}

		template<typename T>
		static bool CompareTypeById(const Styleable& element)
		{
			return typeid(T) == typeid(element);
		}

		void Release();
	private:
		struct Data
		{
			TypeComparer typeComparer {};
			std::string* name {};
			std::vector<std::string>* classes {};
			uint32_t references = 1;

			~Data();
			bool Match(const Styleable& element) const;
		};

	private:
		Data* _data {};		
	};
}