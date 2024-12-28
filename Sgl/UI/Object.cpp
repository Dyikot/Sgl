#include "Object.h"
#include "../Application.h"

namespace Sgl
{
    Object::Object()
    {
        InitProperties();
        InitPropertyChangedMap();
    }

    Object::Object(const Style& style, SDL_FPoint position) noexcept:
        UIElement(style),
        Position(position)
    {
        InitProperties();
        InitPropertyChangedMap();

        _properties = style.Setters;
        style.EventSetters.TrySetEvent(MouseEnterEvent, MouseEnter);
        style.EventSetters.TrySetEvent(MouseLeaveEvent, MouseLeave);
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

    void Object::SetMargin(const Thikness & value)
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

    void Object::OnRender(RenderContext& renderContext)
    {
        if(_isMouseOver && ToolTip)
        {
            ToolTip->OnRender(renderContext);
        }
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
            Application::Current()->SetApplicationCursor();
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
        _properties[WidthProperty] = Any::New<float>();
        _properties[HeightProperty] = Any::New<float>();
        _properties[MinWidthProperty] = Any::New<float>();
        _properties[MinHeightProperty] = Any::New<float>();
        _properties[MaxWidthProperty] = Any::New<float>();
        _properties[MaxHeightProperty] = Any::New<float>();
        _properties[ZIndexProperty] = Any::New<size_t>(1);
        _properties[MarginProperty] = Any::New<Thikness>();
        /*_propertyMap[CursorProperty] = Any::New<Cursor*>(nullptr);
        _propertyMap[ToolTipProperty] = Any::New<IVisual*>(nullptr);*/
        _properties[HorizontalAlignmentProperty] = HorizontalAlignment::Stretch;
        _properties[VerticalAligmentProperty] = VerticalAligment::Stretch;
        _properties[VisibilityProperty] = Visibility::Visible;
    }

    void Object::InitPropertyChangedMap()
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