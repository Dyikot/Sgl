#pragma once

#include <vector>
#include <memory>

#include "Setter.h"

namespace Sgl
{
	class SetterCollection
	{
	public:
		SetterCollection();
		SetterCollection(std::initializer_list<ISetter*> setters);
		explicit SetterCollection(std::vector<Ref<ISetter>> setters);
		SetterCollection(const SetterCollection& other);
		SetterCollection(SetterCollection&& other) noexcept;
		~SetterCollection();

		auto begin() { return _data->setters.begin(); }
		auto begin() const { return _data->setters.begin(); }

		auto end() { return _data->setters.end(); }
		auto end() const { return _data->setters.end(); }

		void Add(Ref<ISetter> setter);
		void Clear();
		size_t Count() const;

		SetterCollection& operator=(const SetterCollection& other);
		SetterCollection& operator=(SetterCollection&& other) noexcept;
	private:
		struct Data
		{
			uint32_t references;
			std::vector<Ref<ISetter>> setters;
		};
		
		void Release();
	private:
		Data* _data {};
	};
}