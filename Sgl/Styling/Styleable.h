#pragma once

#include <typeindex>

#include "Style.h"
#include "../Data/Bindable.h"
#include "../Base/Collections/PackedMap.h"

namespace Sgl
{
	class Styleable : public Bindable
	{
	public:
		using StyleableElementEventHandler = EventHandler<Styleable>;
	public:
		Styleable();

		VisualStateSet States;
		Event<StyleableElementEventHandler> AttachedToLogicalTree;
		Event<StyleableElementEventHandler> DetachedFromLogicalTree;

		void SetName(const std::string& value);
		const std::string& GetName() const { return _name; }

		void SetStyle(Ref<Style> style);
		template<typename T>
		void SetStyleFor(Ref<Style> style);

		Styleable* GetStylingParent() const { return _stylingParent; }
		bool IsAttachedToLogicalTree() const noexcept { return _isAttachedToLogicalTree; }				
	protected:
		~Styleable() = default;
		virtual void SetParent(Styleable* parent);
		virtual void OnAttachedToLogicalTree();
		virtual void OnDetachedFromLogicalTree();
		virtual Ref<Style> CreateDefaultStyle() const;
		Ref<Style> ResolveStyle() const;
		void ApplyStyle();
		void ApplyStateStyle();
	private:
		Ref<Style> _style;
		Styleable* _stylingParent = nullptr;
		bool _isAttachedToLogicalTree = false;
		std::string _name;
		PackedMap<std::type_index, Ref<Style>> _styles;
	};

	template<typename T>
	inline void Styleable::SetStyleFor(Ref<Style> style)
	{
		_styles.Add(typeid(T), std::move(style));
	}
}