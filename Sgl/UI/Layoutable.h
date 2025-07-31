#pragma once

#include "../Base/Layout.h"
#include "../Base/Size.h"
#include "Style/StyleableElement.h"
#include "../Base/Observable/BindableProperty.h"
#include "../Base/Primitives.h"

namespace Sgl
{
	class Layoutable: public StyleableElement
	{
	public:
		template<typename T, typename TInput = T>
		class MeasuredProperty: public BindableProperty<T, TInput>
		{
		private:
			using base = BindableProperty<T, TInput>;
			Layoutable& _owner;
		public:
			MeasuredProperty(Layoutable& owner):
				_owner(owner)
			{}

			MeasuredProperty(Layoutable& owner, TInput value):
				base(value),
				_owner(owner)
			{}

			void OnChanged() override
			{
				base::OnChanged();
				_owner.InvalidateMeasure();
			}

			using base::operator=;
		};

		template<typename T, typename TInput = T>
		class ArrangedProperty: public BindableProperty<T, TInput>
		{
		private:
			using base = BindableProperty<T, TInput>;
			Layoutable& _owner;
		public:
			ArrangedProperty(Layoutable& owner):
				_owner(owner)
			{}

			ArrangedProperty(Layoutable& owner, TInput value):
				base(value),
				_owner(owner)
			{}

			void OnChanged() override
			{
				base::OnChanged();
				_owner.InvalidateArrange();
			}

			using base::operator=;
		};

		MeasuredProperty<float> Width;
		MeasuredProperty<float> Height;
		MeasuredProperty<float> MinWidth;
		MeasuredProperty<float> MinHeight;
		MeasuredProperty<float> MaxWidth;
		MeasuredProperty<float> MaxHeight;
		MeasuredProperty<bool> IsVisible;
		MeasuredProperty<Thickness> Margin;
		ArrangedProperty<VerticalAlignment> VerticalAlignment;
		ArrangedProperty<HorizontalAlignment> HorizontalAlignment;
	protected:
		FRect _bounds;
		FSize _desiredSize;
		Layoutable* _layoutableParent;
	private:
		bool _isArrangeValid;
		bool _isMeasureValid;
	public:
		Layoutable();
		Layoutable(const Layoutable& other);
		Layoutable(Layoutable&& other) noexcept;
		virtual ~Layoutable() = default;

		FSize GetDesiredSize() const { return _desiredSize; }
		FRect GetBounds() const { return _bounds; }
		Layoutable* GetLayoutableParent() const { return _layoutableParent; }
		bool IsArrangeValid() const { return _isArrangeValid; }
		bool IsMeasureValid() const { return _isMeasureValid; }

		virtual void Arrange(FRect rect);
		virtual void Measure(FSize avaliableSize);
		void InvalidateArrange();
		void InvalidateMeasure();
	protected:
		virtual FSize MeasureContent(FSize avaliableSize) { return FSize(); }
		virtual void ArrangeContent(FRect rect) { }
	};
}