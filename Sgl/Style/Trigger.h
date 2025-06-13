#pragma once

#include <vector>
#include "Style.h"

namespace Sgl
{		
	class Trigger
	{
	private:
		bool _isActive = false;
		IStylable& _target;
		Style _style;
	public:
		Trigger(IStylable& target):
			_target(target),
			_style(target)
		{}

		Trigger(const Trigger& other):
			_target(other._target),
			_style(other._style)
		{}

		Trigger(Trigger&& other) noexcept:
			_target(other._target),
			_style(std::move(other._style))
		{}

		void Activate()
		{
			_isActive = true;
			_target.ApplyDefaultStyle();
			_style.Apply();
		}

		void Deactivate()
		{
			_target.ApplyDefaultStyle();
			_target.GetStyle().Apply();
			_isActive = false;
		}

		bool IsActive() const noexcept
		{
			return _isActive;
		}
	};
}