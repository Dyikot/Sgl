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

		Style(const Style& other):
			_target(other._target),
			_setters(other._setters)
		{}

		Style(Style&& other) noexcept:
			_target(other._target),
			_setters(std::move(other._setters))
		{}

		template<Setter... Setters>
		void Use()
		{
			_setters = { Setters... };
			Apply();
		}
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