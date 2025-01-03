#pragma once

#include "ISupportDataBinding.h"
#include "../Object/PropertyManagers.h"

namespace Sgl
{
	enum class BindingMode
	{
		OneWayToTarget,
		OneWayToSource,
		TwoWay
	};

	class BindingBase
	{
	public:
		virtual void UpdateSource() = 0;
		virtual void UpdateTarget() = 0;
		virtual bool TryUpdateSource() = 0;
		virtual bool TryUpdateTarget() = 0;

		virtual ~BindingBase() = default;
	};

	template<typename TMember, typename TData>
	class Binding: public BindingBase
	{
	public:
		std::reference_wrapper<TData> Source;
		std::reference_wrapper<TMember> Target;
		TMember TData::* Member;
		BindingMode Mode;
	public:
		Binding(ISupportDataBinding& source, 
				TMember& target,
				TMember TData::* member,
				BindingMode mode = BindingMode::OneWayToSource):
			Source(static_cast<TData&>(source)),
			Target(target),
			Member(member),
			Mode(mode)
		{}

		void UpdateSource() override
		{
			Source.get().*Member = Target;
		}

		void UpdateTarget() override
		{
			Target.get() = Source.get().*Member;
		}

		bool TryUpdateSource() override
		{
			if(Mode != BindingMode::OneWayToTarget)
			{
				UpdateSource();
				return true;
			}

			return false;
		}

		bool TryUpdateTarget() override
		{
			if(Mode != BindingMode::OneWayToSource)
			{
				UpdateTarget();
				return true;
			}

			return false;
		}
	};
}