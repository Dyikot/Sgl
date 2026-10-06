# Styling

## What is Style?

A **Style** is a reusable set of property values that can be applied to UI elements. Styles separate visual appearance from element logic, enabling consistent theming and easier maintenance.

A `Style` consists of:
- **Selector**: Determines which elements the style applies to
- **Setters**: A collection of property-value pairs to apply
- **States**: Map of setters for a specific visual state

## Setters

Setters are the mechanism by which styles apply values to properties. There are two types:

1. **Setter**: Applies a fixed value directly
2. **ResourceSetter**: Resolves a value from theme resources at runtime

```cpp
// Create a style for all Button elements
Style
{
    .selector = Selector().OfType<Button>(),
    .setters = SetterCollection
    {
        new Setter(Button::MarginProperty, Thickness(5)),
        new ResourceSetter(Button::BackgroundProperty, "ButtonBgColor")
    }
};
```

## Selectors

Selectors determine which elements a style applies to.

### Supported Selector Types

#### Type Selector

Matches elements of the exact specified type.

```cpp
// Matches only TextBlock elements
Selector().OfType<TextBlock>();
```

#### Type Identity Selector

Matches elements of the specified type or any derived type.

```cpp
// Matches Button and all derived types (e.g., ToggleButton)
Selector().Is<Button>();
```

#### Name Selector

Matches elements by their `Name` property.

```cpp
// Matches element with Name == "SubmitButton"
Selector().Name("SubmitButton");
```

#### Class Selector

Matches elements by CSS-like class names. Multiple classes can be specified; all must match.

```cpp
// Matches elements with class "primary"
Selector().Class("primary");

// Matches elements with both "primary" AND "large" classes
Selector().Class("primary").Class("large");

// Usage: Set classes on element
element->SetClasses("primary large");
```

### Combining Selectors

Selectors can be combined to create more specific rules. All conditions must match (AND logic).

```cpp
// Matches ToggleButton elements with class "toggle" and name "ThemeToggle"
Selector().OfType<ToggleButton>().Class("toggle").Name("ThemeToggle");
```

## Visual states

The state is defined by VisualState. Each state corresponds to a specific name, which is specified during registration. The maximum number of states is 64.

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

### Applying Visual States to a Style:
```cpp
Style hoverStyle 
{
    .selector = Selector().OfType<Button>(),
    .setters = SetterCollection 
    {
        new Setter(Button::BackgroundProperty, Colors::Gray)
    },
    .states = 
    {
        { 
            UIElement::OnHover, 
            SetterCollection 
            {
                new Setter(Button::BackgroundProperty, Colors::LightBlue)
            }
        }
    }
};
```

## Style Collections

A **StyleCollection** is a container that holds multiple styles and applies them sequentially to matching elements.

### Style Application Order

Element style obtained by merging matching styles. Child styles can override parent. Style collections are contained at the application, window, and element levels.

```cpp
// First style
Style
{
    .selector = Selector().Class("button"),
    .setters = SetterCollection
    {
        new Setter(Button::BackgroundProperty, Colors::Blue)
    }
};

// Second style - overrides background if both match
Style
{
    .selector = Selector().Class("button").Class("primary"),
    .setters = SetterCollection
    {
        new Setter(Button::BackgroundProperty, Colors::Green)
    }
};
```

## Value Priority
The class `ValueSource` defines the priority when setting style property values. `Default` has the lowest priority, while `VisualState` has the highest.
```cpp
enum class ValueSource : uint8_t
{
    Default,
    Inheritance,
    Style,
    Local,
    VisualState
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