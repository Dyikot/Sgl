#pragma once

#include "Style.h"

namespace Sgl
{	
	class Trigger final
	{
	private:
		Style& _baseClass;
		bool _isActive = false;
	public:
		Style Class;
	public:
		Trigger(Style& baseClass):
			_baseClass(baseClass), 
			Class(baseClass._target)
		{}

		Trigger(const Trigger& other):
			_baseClass(other._baseClass),
			_isActive(other._isActive),
			Class(other.Class)
		{}

		Trigger(Trigger&& other) noexcept:
			_baseClass(other._baseClass),
			_isActive(other._isActive),
			Class(std::move(other.Class))
		{}

		void Activate()
		{
			_isActive = true;
			_baseClass.ResetAndApply();
			Class.Apply();
		}

		void Deactivate()
		{
			_isActive = false;
			_baseClass.ResetAndApply();
		}

		bool IsActive() const noexcept
		{
			return _isActive;
		}
	};
}