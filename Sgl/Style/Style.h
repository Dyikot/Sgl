#pragma once

#include <vector>

namespace Sgl
{		
	class IStyleable
	{
	public:
		virtual ~IStyleable() = default;

		virtual void ResetStyle() = 0;
	};

	class Trigger;

	class Style final
	{
	public:
		using Setter = void(*)(IStyleable&);
	private:
		IStyleable& _target;
		std::vector<Setter> _setters;
	public:
		Style(IStyleable& target):
			_target(target)
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
				setter(_target);
			}
		}

		void ResetAndApply()
		{			
			_target.ResetStyle();
			Apply();
		}

		friend class Trigger;
	};
}