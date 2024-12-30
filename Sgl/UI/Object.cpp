#include "Object.h"
#include "../Application.h"

namespace Sgl
{
    Object::Object():
        Object(SDL_FPoint())
    {}

    Object::Object(SDL_FPoint position) noexcept
    {
        InitProperties();
        InitPropertyChangedEvents();
    }

    void Object::SetWidth(float value)
    {
        _properties[WidthProperty].As<float>() = value;
        OnPropertyChanged(WidthProperty);
    }

    void Object::SetHeight(float value)
    {
        _properties[HeightProperty].As<float>() = value;
        OnPropertyChanged(HeightProperty);
    }

    void Object::SetMinWidth(float value)
    {
        _properties[MinWidthProperty].As<float>() = value;
        OnPropertyChanged(MinWidthProperty);
    }

    void Object::SetMinHeight(float value)
    {
        _properties[MinHeightProperty].As<float>() = value;
        OnPropertyChanged(MinHeightProperty);
    }

    void Object::SetMaxWidth(float value)
    {
        _properties[MaxWidthProperty].As<float>() = value;
        OnPropertyChanged(MaxWidthProperty);
    }

    void Object::SetMaxHeight(float value)
    {
        _properties[MaxHeightProperty].As<float>() = value;
        OnPropertyChanged(MaxHeightProperty);
    }

    void Object::SetZIndex(size_t value)
    {
        _properties[ZIndexProperty].As<size_t>() = value;
        OnPropertyChanged(ZIndexProperty);
    }

    void Object::SetMargin(const Thikness& value)
    {
        _properties[MarginProperty].As<Thikness>() = value;
        OnPropertyChanged(MarginProperty);
    }

    void Object::SetHorizontalAlignment(HorizontalAlignment value)
    {
        _properties[HorizontalAlignmentProperty].As<HorizontalAlignment>() = value;
        OnPropertyChanged(HorizontalAlignmentProperty);
    }

    void Object::SetVerticalAlignment(VerticalAligment value)
    {
        _properties[VerticalAligmentProperty].As<VerticalAligment>() = value;
        OnPropertyChanged(VerticalAligmentProperty);
    }

    void Object::SetVisibility(Visibility value)
    {
        _properties[VisibilityProperty].As<Visibility>() = value;
        OnPropertyChanged(VisibilityProperty);
    }

    void Object::SetStyle(const Style& style)
    {
        UIElement::SetStyle(style);

        _properties = style.Setters;
        style.EventSetters.TrySetEvent(MouseEnterEvent, MouseEnter);
        style.EventSetters.TrySetEvent(MouseLeaveEvent, MouseLeave);
    }

    void Object::OnRender(RenderContext& renderContext)
    {
        if(_isMouseOver && ToolTip)
        {
            ToolTip->OnRender(renderContext);
        }
    }

    void Object::AddPropertyChangedHandler(PropertyId id, PropertyChangedEventHandler&& handler)
    {
        _propertyChangedEventMap[id] += std::move(handler);
    }

    void Object::AddPropertyChangedHandler(PropertyId id, const PropertyChangedEventHandler& handler)
    {
        _propertyChangedEventMap[id] += handler;
    }

    void Object::RemovePropertyChangedHandler(PropertyId id, PropertyChangedEventHandler&& handler)
    {
        _propertyChangedEventMap[id] -= std::move(handler);
    }

    void Object::RemovePropertyChangedHandler(PropertyId id, const PropertyChangedEventHandler& handler)
    {
        _propertyChangedEventMap[id] -= handler;
    }

    void Object::OnMouseEnter(const MouseButtonEventArgs& e)
    {
        _isMouseOver = true;

        if(Cursor)
        {
            Application::Current()->SetCursor(*Cursor);
        }

        if(MouseEnter)
        {
            MouseEnter(this, e);
        }        
    }

    void Object::OnMouseLeave(const MouseButtonEventArgs& e)
    {
        _isMouseOver = false;

        if(Cursor)
        {
            Application::Current()->SetDefaultCursor();
        }

        if(MouseLeave)
        {
            MouseLeave(this, e);
        }
    }

    void Object::OnPropertyChanged(PropertyId id)
    {
        const auto& PropertyChanged = _propertyChangedEventMap[id];

        if(PropertyChanged)
        {
            PropertyChanged(this, EventArgs());
        }
    }

    void Object::InitProperties()
    {
        _properties.Add<float>(WidthProperty);
        _properties.Add<float>(HeightProperty);
        _properties.Add<float>(MinWidthProperty);
        _properties.Add<float>(MinHeightProperty);
        _properties.Add<float>(MaxWidthProperty);
        _properties.Add<float>(MinHeightProperty);
        _properties.Add<size_t>(ZIndexProperty, 1);
        _properties.Add<Thikness>(MarginProperty);
        _properties.Add<const Sgl::Cursor*>(CursorProperty, nullptr);
        _properties.Add<IVisual*>(ToolTipProperty, nullptr);
        _properties.Add<HorizontalAlignment>(HorizontalAlignmentProperty, HorizontalAlignment::Stretch);
        _properties.Add<VerticalAligment>(VerticalAligmentProperty, VerticalAligment::Stretch);
        _properties.Add<Visibility>(VisibilityProperty, Visibility::Visible);
    }

    void Object::InitPropertyChangedEvents()
    {
        _propertyChangedEventMap[WidthProperty] = Event<PropertyChangedEventHandler>();
        _propertyChangedEventMap[HeightProperty] = Event<PropertyChangedEventHandler>();
        _propertyChangedEventMap[MinWidthProperty] = Event<PropertyChangedEventHandler>();
        _propertyChangedEventMap[MinHeightProperty] = Event<PropertyChangedEventHandler>();
        _propertyChangedEventMap[MaxWidthProperty] = Event<PropertyChangedEventHandler>();
        _propertyChangedEventMap[MaxHeightProperty] = Event<PropertyChangedEventHandler>();
        _propertyChangedEventMap[ZIndexProperty] = Event<PropertyChangedEventHandler>();
        _propertyChangedEventMap[MarginProperty] = Event<PropertyChangedEventHandler>();
        _propertyChangedEventMap[CursorProperty] = Event<PropertyChangedEventHandler>();
        _propertyChangedEventMap[ToolTipProperty] = Event<PropertyChangedEventHandler>();
        _propertyChangedEventMap[HorizontalAlignmentProperty] = Event<PropertyChangedEventHandler>();
        _propertyChangedEventMap[VerticalAligmentProperty] = Event<PropertyChangedEventHandler>();
        _propertyChangedEventMap[VisibilityProperty] = Event<PropertyChangedEventHandler>();
    }
}