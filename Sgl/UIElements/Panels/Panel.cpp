#include "Panel.h"
#include "../../Layout/LayoutHelper.h"

namespace Sgl
{  
    using const_iterator = UIElementsCollection::const_iterator;
    using const_reverse_iterator = UIElementsCollection::const_reverse_iterator;

    UIElementsCollection::UIElementsCollection(Panel& panel):
        _panel(panel)
    {}

    const_iterator UIElementsCollection::begin() const
    {
        return _panel.GetChildren().begin();
    }

    const_iterator UIElementsCollection::end() const
    {
        return _panel.GetChildren().end();
    }

    const_reverse_iterator UIElementsCollection::rbegin() const
    {
        return _panel.GetChildren().rbegin();
    }

    const_reverse_iterator UIElementsCollection::rend() const
    {
        return _panel.GetChildren().rend();
    }

    size_t UIElementsCollection::Count() const noexcept
    {
        return _panel.GetChildren().size();
    }

    bool UIElementsCollection::IsEmpty() const noexcept
    {
        return _panel.GetChildren().empty();
    }

    const Ref<UIElement>& UIElementsCollection::Front() const
    {
        return _panel.GetChildren().front();
    }

    const Ref<UIElement>& UIElementsCollection::Back() const
    {
        return _panel.GetChildren().back();
    }

    void UIElementsCollection::Add(const Ref<UIElement>& item)
    {
        _panel.AddChild(item);
    }

    void UIElementsCollection::Insert(size_t index, const Ref<UIElement>& item)
    {
        _panel.InsertChild(index, item);
    }

    bool UIElementsCollection::Remove(const Ref<UIElement>& item)
    {
        return _panel.RemoveChild(item);
    }

    void UIElementsCollection::RemoveAt(size_t index)
    {
        _panel.RemoveChildAt(index);
    }

    void UIElementsCollection::Clear()
    {
        _panel.ClearChildren();
    }

    bool UIElementsCollection::Contains(const Ref<UIElement>& item) const
    {
        auto& children = _panel.GetChildren();
        return std::ranges::find(children, item) != children.end();
    }

    Ref<UIElement> UIElementsCollection::FindByName(std::string_view name) const
    {
        for(auto& item : _panel.GetChildren())
        {
            if(item->GetName() == name)
            {
                return item;
            }
        }

        return nullptr;
    }

    void UIElementsCollection::SetElementAt(size_t index, const Ref<UIElement>& item)
    {
        _panel.SetChildAt(index, item);
    }

    const Ref<UIElement>& UIElementsCollection::GetElementAt(size_t index) const
    {
        return _panel.GetChildren().at(index);
    }

    const Ref<UIElement>& UIElementsCollection::operator[](size_t index) const
    {
        return _panel.GetChildren()[index];
    }

    Panel::Panel():
        Children(*this)
    {
        SetName("Panel");
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
}