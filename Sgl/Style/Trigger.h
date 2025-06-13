#pragma once

#include <vector>
#include "Style.h"

namespace Sgl
{		
	class Trigger
	{
	public:
		Style Style;
	private:
		bool _isActive = false;
		IStylable& _target;
	public:
		Trigger(IStylable& target):
			_target(target),
			Style(target)
		{}

		Trigger(const Trigger& other):
			_target(other._target),
			Style(other.Style)
		{}

		Trigger(Trigger&& other) noexcept:
			_target(other._target),
			Style(std::move(other.Style))
		{}

		void Activate()
		{
			_isActive = true;
			_target.ApplyDefaultStyle();
			Style.Apply();
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