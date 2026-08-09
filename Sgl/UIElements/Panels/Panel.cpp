#include "Panel.h"
#include "../../Layout/LayoutHelper.h"

namespace Sgl
{  
    UIElementsCollection::UIElementsCollection(Panel& panel):
        _panel(panel)
    {}

    UIElementsCollection::~UIElementsCollection()
    {
        ClearItems();
    }

    Ref<UIElement> UIElementsCollection::FindByName(std::string_view name) const
    {
        for(auto& item : _items)
        {
            if(item->Name == name)
            {
                return item;
            }
        }

        return nullptr;
    }

    void UIElementsCollection::ClearItems()
    {
        for(auto& item : _items)
        {
            _panel.RemoveChild(item);
        }

        base::ClearItems();
    }

    void UIElementsCollection::InsertItem(size_t index, const Ref<UIElement>& item)
    {
        _panel.AddChild(item);
        base::InsertItem(index, item);
    }

    void UIElementsCollection::SetItem(size_t index, const Ref<UIElement>& item)
    {
        _panel.AddChild(item);
        base::SetItem(index, item);
    }

    void UIElementsCollection::RemoveItem(size_t index)
    {
        auto& item = GetElementAt(index);
        _panel.RemoveChild(item);
        base::RemoveItem(index);
    }

    Panel::Panel():
        UIElement(),
        Children(*this)
    {
        Name = "Panel";
    }

    Panel::Panel(Panel&& other) noexcept:
        UIElement(std::move(other)),
        Children(*this)
    {}

    FSize Panel::MeasureContent(FSize availableSize)
    {
        FSize size {};

        for(auto& child : Children)
        {
            child->Measure(availableSize);

            auto [width, height] = child->GetDesiredSize();
            
            if(width > size.Width)
            {
                size.Width = width;
            }

            if(height > size.Height)
            {
                size.Height = height;
            }
        }

        return size;
    }

    void Panel::ArrangeContent(FRect rect)
    {
        for(auto& child : Children)
        {
            child->Arrange(rect);
        }
    }

    Styleable& Panel::FirstChild::operator()(Styleable& element) const
    {
        return static_cast<Panel&>(element).Children.Front().GetValue();
    }

    Styleable& Panel::LastChild::operator()(Styleable& element) const
    {
        return static_cast<Panel&>(element).Children.Back().GetValue();
    }

    Panel::NthChild::NthChild(size_t position):
        Index(std::max(1ull, position - 1ull))
    {}

    Styleable& Panel::NthChild::operator()(Styleable& element) const
    {
        return static_cast<Panel&>(element).Children.GetElementAt(Index).GetValue();
    }
}