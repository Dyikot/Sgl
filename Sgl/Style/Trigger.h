#pragma once

#include "Style.h"

namespace Sgl
{	
	template<typename TTargetType>
	class Trigger final
	{
	private:
		using Style = Style<TTargetType>;

		Style& _class;
	public:
		Style TriggerStyle;
	public:
		Trigger(Style& classStyle):
			_class(classStyle), 
			TriggerStyle(classStyle._properties)
		{}

		Trigger(const Trigger&) = delete;
		Trigger(Trigger&&) = delete;

		void Activate()
		{
			_class.ResetAndApply();
			TriggerStyle.Apply();
		}

		void Deactivate()
		{
			_class.ResetAndApply();
		}

		Trigger& operator=(const Trigger&) = delete;
		Trigger& operator=(Trigger&&) = delete;
	};
}