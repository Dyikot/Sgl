#pragma once

#include "Style.h"

namespace Sgl
{	
	class Trigger final
	{
	private:
		Style& _class;
		bool _isActive = false;
	public:
		Style TriggerStyle;
	public:
		Trigger(Style& classStyle):
			_class(classStyle), 
			TriggerStyle(classStyle._target)
		{}

		Trigger(const Trigger& other):
			_class(other._class),
			_isActive(other._isActive),
			TriggerStyle(other.TriggerStyle)
		{}

		Trigger(Trigger&& other) noexcept:
			_class(other._class),
			_isActive(other._isActive),
			TriggerStyle(std::move(other.TriggerStyle))
		{}

		void Activate()
		{
			_isActive = true;
			_class.ResetAndApply();
			TriggerStyle.Apply();
		}

		void Deactivate()
		{
			_isActive = false;
			_class.ResetAndApply();
		}

		bool IsActive() const noexcept
		{
			return _isActive;
		}
	};
}