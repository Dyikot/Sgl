# Styling

## What is Style?

A **Style** is an abstract class used to set element properties. Styles separate visual appearance from element logic, enabling consistent theming and easier maintenance.

Style defines two hooks:

- `OnBase` — applies the default, unconditional base styling rules.
- `OnState` — applies state-specific styling rules based on the element's current visual states.

For UI elements, derive from `UIElementStyle` instead of `Style` directly. `UIElementStyle` implements `OnState` and dispatches each built-in state to a dedicated virtual hook — `OnHover`, `OnPressed`, `OnFocus` — so you only override the states you want to customize.

```cpp
//! @brief Abstract base class representing a visual style applied to Styleable elements
class Style : public RefCounted
{
public:
	//! @brief Applies the default, unconditional base styling rules
	//! @param context The style context providing access to the target object and resources
	virtual void OnBase(StyleContext& context) = 0;

	//! @brief Applies state-specific styling rules based on the current element state
	//! @param context The style context providing access to the target object and resources
	//! @param states The set of active visual states
	virtual void OnState(StyleContext& context, const VisualStateSet& states) = 0;
};
```

## Applying a style
A style is set with the SetStyle method:
`button.SetStyle(MakeRef<ButtonStyle>());`

`SetStyle` does not apply the style immediately. Styles are applied when the element is attached to the UI tree, and re-applied when the active theme changes.

`Style` is reference-counted and stateless, so a single instance can safely be shared between any number of elements. 

If no style is set, the element uses its default style, created by the `CreateDefaultStyle` virtual method. Override it in a custom element to provide your own default:
```cpp
class MyButton : public Button
{
protected:
    Ref<Style> CreateDefaultStyle() const override
    {
        return New<MyButtonStyle>();
    }
};
```

## StyleContext
Properties are set via a special proxy class `StyleContext`.
The style context allows you to set a value directly or via application resources by specifying the required key. Currently, resources support colors and brushes. Resource keys are resolved against the resources of the current application instance.

```cpp
context.Set(Button::BackgroundProperty, Colors::Blue); // Direct value
context.Set(Button::BackgroundProperty, "ButtonBg");   // Value from application resources
```

## Visual states
A state is defined by VisualState. Each state has a name, specified during registration, and a unique numeric ID. Registration is global to the application: IDs are assigned sequentially from 0, and no more than 64 states can be registered.

Built-in visual states:
| Name | VisualState | Description |
|-----------|--------------------------|------------------------------------------|
| `hover` | `UIElement::OnHover` | Mouse is over the element |
| `pressed` | `UIElement::OnPressed` | Mouse button is pressed on the element |
| `focus` | `UIElement::OnFocus` | Element got focus |
| `checked` | `ToggleButton::OnChecked` | Toggle state is active |

To register a visual state, you must use the `VisualState::Register` static method.

### Supported Properties
Next are the properties that can be used to style visual states:
- `Renderable::BackgroundProperty`
- `Renderable::CursorProperty`
- `Border::BorderColorProperty`
- `Border::BorderWidthProperty`
- `TextBlock::ForegroundProperty`

## Dispatch order
When several states are active at once, the default `UIElementStyle::OnState` dispatches hooks in a fixed order — hover, focus, pressed:
```cpp
void UIElementStyle::OnState(StyleContext& context, const VisualStateSet& states)
{
	if(states.Has(UIElement::OnHover))
	{
		OnHover(context);
	}

	if(states.Has(UIElement::OnFocus))
	{
		OnFocus(context);
	}

	if(states.Has(UIElement::OnPressed))
	{
		OnPressed(context);
	}
}
```

The order can be changed by overriding `OnState`.

## Example
The default style `ButtonStyle` for a button:
```cpp
class ButtonStyle : public UIElementStyle
{
public:
	void OnBase(StyleContext& context) override
    {
        context.Set(Button::CursorProperty, Cursors::Pointer);
        context.Set(Button::BackgroundProperty, Colors::Grey_2);
    }

protected:
	void OnHover(StyleContext& context) override
    {
        context.Set(Button::BackgroundProperty, Colors::Grey_3);
    }

	void OnPressed(StyleContext& context) override
    {
        context.Set(Button::BackgroundProperty, Colors::Grey_4);
    }

	void OnFocus(StyleContext& context) override
    {

    }
};
```

## Custom visual states
To style a state that has no dedicated hook (e.g. checked), override `OnState`, call thebase dispatch to keep the built-in hooks running, and route the new state to its own hook. The default `ToggleButtonStyle` does exactly this:
```cpp
class ToggleButtonStyle : public ButtonStyle
{
public:
	void OnState(StyleContext& context, const VisualStateSet& states) override
    {
        ButtonStyle::OnState(context, states);

        if(states.Has(ToggleButton::OnChecked))
        {
            OnChecked(context);
        }
    }
protected:
	void OnHover(StyleContext& context) override {}
	void OnPressed(StyleContext& context) override {}
	void OnFocus(StyleContext& context) override {}
	virtual void OnChecked(StyleContext& context) {}
};
```

## Styling composite elements
Complex elements such as `ProgressBar` are built from internal elements (parts) that are not exposed as properties. Parts are regular elements: find one by name with `FindChild` and set a style on it as you would on any element.
```cpp
auto progressBar = New<ProgressBar>();
progressBar->FindChild("Track")->SetStyle(New<MyTrackStyle>());
```

## Value Priority
The class `ValueSource` defines the priority when setting style property values. `Default` has the lowest priority, while `Local` has the highest.
```cpp
enum class ValueSource : uint8_t
{
    Default,
    Inheritance,
    Style,
    VisualState,
    Local
};
```

## Theme Styling

Theme styling enables UI elements to automatically adapt their appearance based on the active theme (Light or Dark). This is achieved through theme-aware resources.

### ThemeMode and ThemeVariant

`ThemeMode` is used by resources and determines the current theme. `ThemeVariant` is used by the application and automatically sets the theme mode for resources.

```cpp
enum class ThemeMode
{
    Light,  // Light theme
    Dark    // Dark theme
};

enum class ThemeVariant
{
    Light,   // Force light theme
    Dark,    // Force dark theme
    System   // Follow system theme
};
```

### ThemeResources

`ThemeResources` manages themed color and brush resources.

```cpp
// Add themed colors
app.Resources.AddColor(
    "PrimaryColor",
    Colors::Blue,        // Light theme
    Colors::LightBlue    // Dark theme
);

// Add themed brushes
app.Resources.AddBrush(
    "BackgroundBrush",
    Colors::White,       // Light theme
    Colors::Black        // Dark theme
);
```

### Switching Themes

```cpp
// Set current theme
app.SetThemeVariant(ThemeVariant::Dark);

// Retrieve themed resources (automatically uses current theme)
Brush bgBrush = app.Resources.GetBrush("BackgroundBrush");
```