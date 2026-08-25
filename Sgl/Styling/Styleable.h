#pragma once

#include "IStyleHost.h"
#include "PseudoClass.h"
#include "../Data/Bindable.h"

namespace Sgl
{
	class Styleable : public Bindable, public IStyleHost
	{
	public:
		using StyleableElementEventHandler = EventHandler<Styleable>;
	public:
		Styleable();
		Styleable(Styleable&& other) noexcept;

		std::string Name;
		StyleCollection Styles;
		PseudoClassesSet PseudoClasses;
		Event<StyleableElementEventHandler> AttachedToLogicalTree;
		Event<StyleableElementEventHandler> DetachedFromLogicalTree;

		void SetClasses(std::string_view classNames);
		void SetClasses(std::vector<std::string> classList);
		const std::vector<std::string>& GetClasses() const;

		StyleCollection& GetStyles() final;
		IStyleHost* GetStylingParent() final { return _stylingParent; }
		bool IsAttachedToLogicalTree() const noexcept { return _isAttachedToLogicalTree; }

		virtual void ApplyStyle();
	protected:
		~Styleable() = default;
		virtual void SetParent(IStyleHost* parent);
		virtual void OnAttachedToLogicalTree();
		virtual void OnDetachedFromLogicalTree();
	private:
		bool FetchStyles();
		void FetchStylesFrom(const StyleCollection& styles);
		void OnStyleClassesChanged();
		void ApplyStateStyle();
		void SaveBaseState();
		void RestoreBaseState();
		void ClearMatchingStateStyles();
		bool MatchStateStyles();
	private:
		std::vector<std::string> _classList;
		std::vector<const Style*> _styles;
		std::vector<const Style*> _stateStyles;
		std::vector<const Style*> _matchingStateStyles;
		std::vector<std::unique_ptr<IPropertyStateGuard>> _propertyGuards;
		IStyleHost* _stylingParent = nullptr;
		bool _isAttachedToLogicalTree = false;
	};

	class IPropertyStateGuard
	{
	public:
		virtual ~IPropertyStateGuard() = default;
	};

	template<typename TOwner, typename TValue>
	class PropertyStateGuard : public IPropertyStateGuard
	{
	public:
		using Value = std::decay_t<TValue>;
	public:
		PropertyStateGuard(StyleableProperty<TOwner, TValue>& property, TOwner& target):
			_property(property),
			_target(&target),
			_value(property.InvokeGetter(target))
		{}

		~PropertyStateGuard()
		{
			_property.InvokeSetter(_target.GetValue(), _value, ValueSource::PseudoClass);
		}
	private:
		StyleableProperty<TOwner, TValue>& _property;
		Ref<TOwner> _target;
		Value _value;
	};

	template<typename TOwner, typename TValue>
	inline IPropertyStateGuard* StyleableProperty<TOwner, TValue>::CreateStateGuard(Styleable& target)
	{
		return new PropertyStateGuard(*this, static_cast<TOwner&>(target));
	}
}