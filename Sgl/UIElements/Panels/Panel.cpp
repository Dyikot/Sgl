#include "Panel.h"
#include "../../Layout/LayoutHelper.h"

namespace Sgl
{  
    UIElementsCollection::UIElementsCollection(Panel& owner):
        _owner(owner)
    {}

    UIElementsCollection::UIElementsCollection(UIElementsCollection&& other) noexcept:
        base(std::move(other)),
        _owner(other._owner)
    {}

    UIElementsCollection::~UIElementsCollection()
    {
        ClearItems();
    }

    void UIElementsCollection::ClearItems()
    {
        for(auto& item : _items)
        {
            _owner.OnChildRemoving(item.GetValue());
        }

        base::ClearItems();
    }

    void UIElementsCollection::InsertItem(size_t index, const Ref<UIElement>& item)
    {
        _owner.OnChildAdded(item.GetValue());
        base::InsertItem(index, item);
    }

    void UIElementsCollection::SetItem(size_t index, const Ref<UIElement>& item)
    {
        _owner.OnChildAdded(item.GetValue());
        base::SetItem(index, item);
    }

    void UIElementsCollection::RemoveItem(size_t index)
    {
        auto& item = GetElementAt(index);
        _owner.OnChildRemoving(item.GetValue());

        base::RemoveItem(index);
    }

    Panel::Panel():
        UIElement(),
        Children(*this)
    {}

    Panel::Panel(Panel&& other) noexcept:
        UIElement(std::move(other)),
        Children(std::move(Children)),
        _currentChild(std::exchange(other._currentChild, nullptr))
    {}

    void Panel::SetVisualRoot(IVisualRoot* value)
    {
        UIElement::SetVisualRoot(value);

        for(auto& child : Children)
        {
            child->SetVisualRoot(value);
        }
    }

    void Panel::Render(RenderContext context)
    {
        RenderBackground(context, _bounds);

        for(auto& child : Children)
        {
            if(child->IsVisible())
            {
                child->Render(context);
            }
        }

        UIElement::Render(context);
    }

    void Panel::ApplyStyle()
    {
        StyleableElement::ApplyStyle();

        for(auto& child : Children)
        {
            child->ApplyStyle();
        }
    }

    void Panel::OnChildAdded(UIElement& child)
    {
        child.SetParent(this);
        
        if(IsAttachedToLogicalTree())
        {
            child.OnAttachedToLogicalTree();
        }
    }

    void Panel::OnChildRemoving(UIElement& child)
    {
        if(IsAttachedToLogicalTree())
        {
            child.OnDetachedFromLogicalTree();
        }

        child.SetParent(nullptr);
    }    

    void Panel::OnAttachedToLogicalTree()
    {
        UIElement::OnAttachedToLogicalTree();

        for(auto& child : Children)
        {
            child->OnAttachedToLogicalTree();
        }
    }

    void Panel::OnDetachedFromLogicalTree()
    {
        UIElement::OnDetachedFromLogicalTree();

        for(auto& child : Children)
        {
            child->OnDetachedFromLogicalTree();
        }
    }

    void Panel::OnCursorChanged(const Cursor& cursor)
    {
        UIElement::OnCursorChanged(cursor);

        for(auto& child : Children)
        {
            child->SetCursor(cursor, ValueSource::Inheritance);
        }
    }

    void Panel::OnDataContextChanged(const Ref<INotifyPropertyChanged>& dataContext)
    {
        for(auto& child : Children)
        {
            child->SetDataContext(dataContext, ValueSource::Inheritance);
        }
    }

    void Panel::OnMouseMove(MouseMoveEventArgs e)
    {
        UIElement::OnMouseMove(e);

        auto& current = _currentChild;

        if(current)
        {   
            if(current->IsVisible() && LayoutHelper::IsPointInRect(e.X, e.Y, current->GetBounds()))
            {
                current->OnMouseMove(e);
                return;
            }
            else
            {
                current->OnMouseLeave(e);
                current = nullptr;
            }
        }

        for(auto& child : Children)
        {
            if(!child->IsVisible())
            {
                continue;
            }

            if(LayoutHelper::IsPointInRect(e.X, e.Y, child->GetBounds()))
            {
                Cursor::Set(child->GetCursor());

                current = child;
                child->OnMouseEnter(e);
                child->OnMouseMove(e);
                return;
            }
        }        

        Cursor::Set(GetCursor());
    }

    void Panel::OnMouseDown(MouseButtonEventArgs e)
    {
        UIElement::OnMouseDown(e);

        if(_currentChild && _currentChild->IsVisible() && _currentChild->IsMouseOver())
        {
            _mouseCapturedElement = _currentChild;
            _currentChild->OnMouseDown(e);
        }
    }

    void Panel::OnMouseUp(MouseButtonEventArgs e)
    {
        UIElement::OnMouseUp(e);

        if(_mouseCapturedElement)
        {
            _mouseCapturedElement->OnMouseUp(e);
            _mouseCapturedElement = nullptr;
        }
        else if(_currentChild && _currentChild->IsVisible() && _currentChild->IsMouseOver())
        {
            _currentChild->OnMouseUp(e);
        }
    }

    void Panel::OnMouseLeave(MouseMoveEventArgs e)
    {
        UIElement::OnMouseLeave(e);

        if(_currentChild && _currentChild->IsVisible())
        {
            _currentChild->OnMouseLeave(e);
            _currentChild = nullptr;
        }
    }

    StyleableElement& Panel_FirstChild::operator()(StyleableElement& target) const
    {
        return static_cast<Panel&>(target).Children.Front().GetValue();
    }

    StyleableElement& Panel_LastChild::operator()(StyleableElement& target) const
    {
        return static_cast<Panel&>(target).Children.Back().GetValue();
    }

    StyleableElement& Panel_NthChild::operator()(StyleableElement& target) const
    {
        return static_cast<Panel&>(target).Children.GetElementAt(Index).GetValue();
    }
}