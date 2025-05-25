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
		Style Class;
	public:
		Trigger(Style& classStyle):
			_class(classStyle), 
			Class(classStyle._target)
		{}

		Trigger(const Trigger& other):
			_class(other._class),
			_isActive(other._isActive),
			Class(other.Class)
		{}

		Trigger(Trigger&& other) noexcept:
			_class(other._class),
			_isActive(other._isActive),
			Class(std::move(other.Class))
		{}

		void Activate()
		{
			_isActive = true;
			_class.ResetAndApply();
			Class.Apply();
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