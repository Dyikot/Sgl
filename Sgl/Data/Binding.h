#pragma once

#include "../Object/ObjectId.h"
#include "../Events/Delegates.h"
#include <string>

namespace Sgl
{
	using NotifySource = Action<PropertyId, const Any&>;
	using NotifyTarget = Action<const Any&>;

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
		bool _beingTargetNotified = false;
	public:
		void NotifyTarget(const Any& value)
		{
			if(Target)
			{
				_beingTargetNotified = true;
				Target(value);
				_beingTargetNotified = false;
			}
		}

		void NotifySource(PropertyId id, const Any& value)
		{
			if(Source && !_beingTargetNotified)
			{
				Source(id, value);
			}
		}
	};
}