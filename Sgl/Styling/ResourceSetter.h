#pragma once

#include <string>

#include "Setter.h"
#include "../Base/Media/Brush.h"

namespace Sgl
{
	//! @brief Base class for resource setter 
	class ResourceSetterBase : public ISetter
	{
	public:
		explicit ResourceSetterBase(std::string key);
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
			ResourceSetterBase(std::move(key)),
			_property(property)
		{}

		//! @brief Gets target property
		//! @return Reference to property
		PropertyBase& GetProperty() const override
		{
			return _property;
		}

		//! @brief Applies the value from resources to the specified target element
		//! @param target The target element
		//! @param valueSource The source of the value
		void Apply(Styleable& target, ValueSource valueSource) const override
		{
			_property.InvokeSetter(static_cast<TOwner&>(target), GetColor(), valueSource);
		}
	private:
		ColorProperty& _property;
	};

	//! @brief A setter that resolves a brush from a theme resource at runtime
	template<typename TOwner>
	class ResourceSetter<TOwner, const Brush&> final : public ResourceSetterBase
	{
	public:
		using BrushProperty = StyleableProperty<TOwner, const Brush&>;
	public:
		ResourceSetter(BrushProperty& property, std::string key):
			ResourceSetterBase(std::move(key)),
			_property(property)
		{}

		//! @brief Gets target property
		//! @return Reference to property
		PropertyBase& GetProperty() const override
		{
			return _property;
		}

		//! @brief Applies the value from resources to the specified target element
		//! @param target The target element
		//! @param valueSource The source of the value
		void Apply(Styleable& target, ValueSource valueSource) const override
		{
			_property.InvokeSetter(static_cast<TOwner&>(target), GetBrush(), valueSource);
		}
	private:
		BrushProperty& _property;
	};

	template<typename TOwner, typename TValue>
	ResourceSetter(StyleableProperty<TOwner, TValue>&, std::string) -> ResourceSetter<TOwner, TValue>;
}