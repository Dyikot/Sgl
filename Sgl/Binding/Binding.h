#pragma once

#include <string>
#include <functional>
#include "BindableObject.h"

namespace Sgl
{
	using NotifySource = std::function<void(PropertyId, const Any&)>;
	using NotifyTarget = std::function<void(const Any&)>;

	class ISupportSourceBinding
	{
	public:
		virtual ~ISupportSourceBinding() = default;

		virtual NotifySource& GetNotifySource() = 0;
	};

	enum class BindingMode
	{
		OneWayToTarget,
		OneWayToSource,
		TwoWay
	};
	
	class Binding
	{
	public:
		NotifySource Source;
		NotifyTarget Target;
	private:
		bool _targetNotifying = false;
	public:
		void NotifyTarget(const Any& value)
		{
			if(Target)
			{
				_targetNotifying = true;
				Target(value);
				_targetNotifying = false;
			}
		}

		void NotifySource(PropertyId id, const Any& value)
		{
			if(Source && !_targetNotifying)
			{
				Source(id, value);
			}
		}
	};
}