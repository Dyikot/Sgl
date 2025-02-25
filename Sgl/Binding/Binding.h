#pragma once

#include <string>
#include <functional>
#include "BindableObject.h"

namespace Sgl
{
	using SourceNotifier = std::function<void(PropertyId)>;
	using TargetNotifier = std::function<void(const Any&)>;

	class ISupportComponentBinding
	{
	public:
		virtual ~ISupportComponentBinding() = default;

		virtual SourceNotifier& GetSourceNotifier() = 0;
	};
	
	class Binding
	{
	public:
		SourceNotifier Source;
		TargetNotifier Target;
	private:
		bool _lock = false;
	public:
		void NotifyTarget(const Any& value)
		{
			if(Target)
			{
				_lock = true;
				Target(value);
				_lock = false;
			}
		}

		void NotifySource(PropertyId id)
		{
			if(Source && !_lock)
			{
				Source(id);
			}
		}
	};
}