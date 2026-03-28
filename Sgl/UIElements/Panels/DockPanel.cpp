#include "DockPanel.h"

namespace Sgl::UIElements
{
	LayoutProperty<Dock> DockPanel::DockProperty =
	{
		[](Layoutable& element, Dock value)
		{
			element.SetProperty(DockProperty, element.GetLayoutContext<Dock>(), value);
		},
		[](Layoutable& element) { return element.GetLayoutContext<Dock>(); }
	};

	void DockPanel::SetDock(const Ref<UIElement>& element, Dock dock)
	{
		element->SetProperty(DockProperty, element->GetLayoutContext<Dock>(), dock);
	}

	void DockPanel::SetLastChildFill(bool value, ValueSource source)
	{
		if(SetProperty(LastChildFillProperty, _lastChildFill, value, _lastChildFillSource, source))
		{
			InvalidateMeasure();
		}
	}

	FSize DockPanel::MeasureContent(FSize availableSize)
	{
		if(Children.Count() == 0)
		{
			return {};
		}

		FSize remainingSize = availableSize;
		auto childrenCount = _lastChildFill ? Children.Count() - 1 : Children.Count();

		for(size_t i = 0; i < childrenCount; i++)
		{
			auto& child = Children[i];

			if(!child->IsVisible())
			{
				continue;
			}

			child->Measure(remainingSize);
			auto [width, height] = child->GetDesiredSize();

			switch(GetDock(child))
			{
				case Dock::Left:
				case Dock::Right:
				{
					remainingSize.Width -= width;
					break;
				}

				case Dock::Top:
				case Dock::Bottom:
				{
					remainingSize.Height -= height;
					break;
				}
			}
		}

		FSize desiredSize = availableSize - remainingSize;

		if(_lastChildFill)
		{
			auto& lastChild = Children.Back();

			if(lastChild->IsVisible())
			{
				lastChild->Measure(remainingSize);
				auto [width, height] = lastChild->GetDesiredSize();
				desiredSize.Width = std::max(desiredSize.Width, width);
				desiredSize.Height = std::max(desiredSize.Height, height);
			}
		}

		return desiredSize;
	}

	void DockPanel::ArrangeContent(FRect rect)
	{
		if(Children.Count() == 0)
		{
			return;
		}
		
		auto remainingRect = rect;
		auto childrenCount = _lastChildFill ? Children.Count() - 1 : Children.Count();

		for(size_t i = 0; i < childrenCount; i++)
		{
			auto& child = Children[i];
			
			if(!child->IsVisible())
			{
				continue;
			}

			float width = 0, height = 0;
			switch(GetDock(child))
			{
				case Dock::Left:
				{
					width = std::min(child->GetDesiredSize().Width, remainingRect.w);
					child->Arrange({
						.x = remainingRect.x,
						.y = remainingRect.y,
						.w = width,
						.h = remainingRect.h
					});

					remainingRect = {
						.x = remainingRect.x + width,
						.y = remainingRect.y,
						.w = remainingRect.w - width,
						.h = remainingRect.h
					};

					break;
				}

				case Dock::Right:
				{
					width = std::min(child->GetDesiredSize().Width, remainingRect.w);
					child->Arrange({
						.x = remainingRect.x + remainingRect.w - width,
						.y = remainingRect.y,
						.w = width,
						.h = remainingRect.h
					});

					remainingRect = {
						.x = remainingRect.x,
						.y = remainingRect.y,
						.w = remainingRect.w - width,
						.h = remainingRect.h
					};

					break;
				}

				case Dock::Top:
				{
					height = std::min(child->GetDesiredSize().Height, remainingRect.h);
					child->Arrange({
						.x = remainingRect.x,
						.y = remainingRect.y,
						.w = remainingRect.w,
						.h = height
					});

					remainingRect = {
						.x = remainingRect.x,
						.y = remainingRect.y + height,
						.w = remainingRect.w,
						.h = remainingRect.h - height
					};

					break;
				}

				case Dock::Bottom:
				{
					height = std::min(child->GetDesiredSize().Height, remainingRect.h);
					child->Arrange({
						.x = remainingRect.x,
						.y = remainingRect.y + remainingRect.h - height,
						.w = remainingRect.w,
						.h = height
					});

					remainingRect = {
						.x = remainingRect.x,
						.y = remainingRect.y,
						.w = remainingRect.w,
						.h = remainingRect.h - height
					};

					break;
				}
			}
		}

		if(_lastChildFill)
		{
			auto& lastChild = Children.Back();

			if(lastChild->IsVisible())
			{
				lastChild->Arrange({
					.x = remainingRect.x,
					.y = remainingRect.y,
					.w = remainingRect.w,
					.h = remainingRect.h
				});
			}
		}
	}
}