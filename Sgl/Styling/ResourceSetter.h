#pragma once

#include <string>

#include "Setter.h"
#include "../Base/Media/Brush.h"

namespace Sgl
{
	//! @brief Base class for resource setter 
	class ResourceSetterBase : public SetterBase
	{
	public:
		ResourceSetterBase(PropertyBase& property, std::string key);
	protected:
		Color GetColor() const;
		Brush GetBrush() const;
	private:
		std::string _key;
	};

	//! @brief A setter that resolves a value from a theme resource at runtime
	template<typename TOwner, typename TValue>
	class ResourceSetter;

	//! @brief A setter that resolves a color from a theme resource at runtime
	template<typename TOwner>
	class ResourceSetter<TOwner, Color> final : public ResourceSetterBase
	{
	public:
		using ColorProperty = StyleableProperty<TOwner, Color>;
	public:
		ResourceSetter(ColorProperty& property, std::string key):
			ResourceSetterBase(property, std::move(key))
		{}

		//! @brief Applies the value from resources to the specified target element
		//! @param target The target element
		//! @param valueSource The source of the value
		void Apply(Styleable& target, ValueSource valueSource) const override
		{
			SetPropertyValue<TOwner, Color>(target, valueSource, GetColor());
		}
	};

	//! @brief A setter that resolves a brush from a theme resource at runtime
	template<typename TOwner>
	class ResourceSetter<TOwner, const Brush&> final : public ResourceSetterBase
	{
	public:
		using BrushProperty = StyleableProperty<TOwner, const Brush&>;
	public:
		ResourceSetter(BrushProperty& property, std::string key):
			ResourceSetterBase(property, std::move(key))
		{}

		//! @brief Applies the value from resources to the specified target element
		//! @param target The target element
		//! @param valueSource The source of the value
		void Apply(Styleable& target, ValueSource valueSource) const override
		{
			SetPropertyValue<TOwner, const Brush&>(target, valueSource, GetBrush());
		}
	};

	template<typename TOwner, typename TValue>
	ResourceSetter(StyleableProperty<TOwner, TValue>&, std::string) -> ResourceSetter<TOwner, TValue>;
}