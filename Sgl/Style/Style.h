#pragma once

#include <vector>

namespace Sgl
{		
	template<typename T>
	struct StyleProperties;

	template<typename TTargetType>
	class Trigger;

	template<typename TTargetType>
	class Style final
	{
	public:
		using Properties = StyleProperties<TTargetType>;
		using Setter = void(*)(Properties&);
	private:
		Properties& _properties;
		std::vector<Setter> _setters;
	public:
		Style(Properties& properties):
			_properties(properties)
		{}

		Style(const Style&) = delete;
		Style(Style&&) = delete;

		void Use(std::vector<Setter> setters)
		{
			_setters = std::move(setters);
			Apply();
		}

		template<Setter... Setters>
		void Use()
		{
			Use({ Setters... });
		}

		Style& operator=(const Style&) = delete;
		Style& operator=(Style&&) = delete;
	private:
		void Apply()
		{
			for(Setter setter : _setters)
			{
				setter(_properties);
			}
		}

		void ResetAndApply()
		{			
			_properties = {};
			Apply();
		}

		friend class Trigger<TTargetType>;
	};
}