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
            _panel.OnChildRemoving(item.Get());
        }

        base::ClearItems();
    }

    void UIElementsCollection::InsertItem(size_t index, const Ref<UIElement>& item)
    {
        _panel.OnChildAdded(item.Get());
        base::InsertItem(index, item);
    }

    void UIElementsCollection::SetItem(size_t index, const Ref<UIElement>& item)
    {
        _panel.OnChildAdded(item.Get());
        base::SetItem(index, item);
    }

    void UIElementsCollection::RemoveItem(size_t index)
    {
        auto& item = GetElementAt(index);
        _panel.OnChildRemoving(item.Get());
        base::RemoveItem(index);
    }

    Panel::Panel():
        UIElement(),
        Children(*this)
    {}

    Panel::Panel(Panel&& other) noexcept:
        UIElement(std::move(other)),
        Children(*this)
    {}

    void Panel::OnChildAdded(UIElement* child)
    {
        AddLogicalChild(child);
    }

    void Panel::OnChildRemoving(UIElement* child)
    {
        RemoveLogicalChild(child);
    }

    std::span<const Ref<UIElement>> Panel::GetChildren() const
    {
        return Children;
    }

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

    StyleableElement& Panel_FirstChild::operator()(StyleableElement& element) const
    {
        return static_cast<Panel&>(element).Children.Front().GetValue();
    }

    StyleableElement& Panel_LastChild::operator()(StyleableElement& element) const
    {
        return static_cast<Panel&>(element).Children.Back().GetValue();
    }

    Panel_NthChild::Panel_NthChild(size_t position):
        Index(std::max(1ull, position - 1ull))
    {}

    StyleableElement& Panel_NthChild::operator()(StyleableElement& element) const
    {
        return static_cast<Panel&>(element).Children.GetElementAt(Index).GetValue();
    }
}