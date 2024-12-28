#pragma once

#include "../Events/Event.h"
#include "../Any.h"
#include "../Appearance/Style/UniquePropertyManager.h"
#include <format>

namespace Sgl
{
	template<typename TId>
	class SetterMap: protected std::unordered_map<TId, Any>
	{
	public:
		using Base = std::unordered_map<TId, Any>;
	public:
		Base::iterator begin() { return Base::begin(); }
		Base::iterator end() { return Base::end(); }
		Base::const_iterator begin() const { return Base::begin(); }
		Base::const_iterator end() const { return Base::end(); }
		Any& At(TId id) { return Base::at(id); }
		const Any& At(TId id) const { return Base::at(id); }
		bool Empty() const { return Base::empty(); }
		size_t Count() const { return Base::size(); }
		bool Contains(TId id) const { return Base::contains(id); }
		void Remove(TId id) { Base::erase(id); }

		void Add(TId id, Any&& object)
		{
			if(!IsTypeCorrect(id, object))
			{
				throw std::invalid_argument(
					std::format("Property type <{}> is not equal object type <{}>\n",
								UniquePropertyManager<TId>::GetTypeNameOf(id),
								object.Type().name()));
			}

			Base::emplace(id, std::move(object));
		}

		void Add(TId id, const Any& object)
		{
			if(!IsTypeCorrect(id, object))
			{
				throw std::invalid_argument(
					std::format("Property type <{}> is not equal object type <{}>\n",
								UniquePropertyManager<TId>::GetTypeNameOf(id),
								object.Type().name()));
			}

			Base::emplace(id, object);
		}

		template<typename TValue, typename... TArgs>
		void Add(TId id, TArgs&&... args)
		{
			Add(id, Any::New<TValue>(std::forward<TArgs>(args)...));
		}

		template<typename TValue>
		bool TryGetValue(TId id, TValue& value) const
		{
			if(Contains(id))
			{
				value = At(id).As<TValue>();
				return true;
			}

			return false;
		}

		SetterMap<TId>& operator=(const SetterMap<TId>& setterMap)
		{
			for(auto& [id, value] : setterMap)
			{
				operator[](id) = value;
			}

			return *this;
		}

		Any& operator[](const TId& id) { return Base::operator[](id); }
	private:
		bool IsTypeCorrect(TId id, const Any& object) const
		{
			return UniquePropertyManager<TId>::GetTypeNameOf(id) == object.Type().name();
		}
	};	
}