# Styling

## What is Style?

A **Style** is a reusable set of property values that can be applied to UI elements. Styles separate visual appearance from element logic, enabling consistent theming and easier maintenance.

A `Style` consists of:
- **Selector**: Determines which elements the style applies to
- **TargetSelector** (optional): Specifies which part of a composite element to style
- **Setters**: A collection of property-value pairs to apply

## Setters

Setters are the mechanism by which styles apply values to properties. There are two types:

1. **ValueSetter**: Applies a fixed value directly
2. **ResourceSetter**: Resolves a value from theme resources at runtime

```cpp
// Create a style for all Button elements
Style().OfType<Button>()
    // Set a fixed value (ValueSetter is created)
    .Set(Button::MarginProperty, Thickness(5))
    // Set a themed resource (ResourceSetter is created)
    .Set(Button::BackgroundProperty, ResourceKey("ButtonBgColor"));
```

## Selectors

Selectors determine which elements a style applies to.

### Supported Selector Types

#### Type Selector

Matches elements of the exact specified type.

```cpp
// Matches only TextBlock elements
Style().OfType<TextBlock>();
```

#### Type Identity Selector

Matches elements of the specified type or any derived type.

```cpp
// Matches Button and all derived types (e.g., ToggleButton)
Style().Is<Button>();
```

#### Name Selector

Matches elements by their `Name` property.

```cpp
// Matches element with Name == "SubmitButton"
Style().Name("SubmitButton");
```

#### Class Selector

Matches elements by CSS-like class names. Multiple classes can be specified; all must match.

```cpp
// Matches elements with class "primary"
Style().Class("primary");

// Matches elements with both "primary" AND "large" classes
Style().Class("primary").Class("large");

// Usage: Set classes on element
element->SetClasses("primary large");
```

#### Pseudo-Class Selector

Matches elements in a specific state. Pseudo-classes are registered by name. The maximum number of states is 64.

```cpp
// Matches elements in the "hover" state
Style().On("hover");

// Matches elements in multiple states (all must be active)
Style().On("hover").On("pressed");
```

Built-in pseudo-classes:
- `hover` - Mouse is over the element
- `pressed` - Mouse button is pressed on the element
- `focus` - Element got focus
- `checked` - Toggle state is active

To register a pseudo-class, you must to use the `PseudoClass::Register` static method.

### Combining Selectors

Selectors can be combined to create more specific rules. All conditions must match (AND logic).

```cpp
// Matches ToggleButton elements with class "toggle" and name "ThemeToggle"
Style().OfType<ToggleButton>().Class("toggle").Name("ThemeToggle");

// Matches Button elements in the pressed state
Style().Is<Button>().Class("primary").On("pressed");
```

## Target selector

Target selectors allow styles to target specific parts of composite elements. Instead of styling the element itself, a selector redirects the style application to a child or internal part.

A `TargetSelector` is a callable that takes a `Styleable&` and returns a `Styleable&` to style. Can be set via `Target` method.

Built-in target selectors:
- `UIElement::Child`
- `Window::Content`
- `Panel::FirstChild`
- `Panel::LastChild`
- `Panel::NthChild`
- `Panel::ChildWithName`
- `Panel::ChildOfType<T>`

```cpp
Style()
    .Is<Button>().Class("TextButton")
    .Target(Button::ContentPresenter())
    .Set(TextBlock::FontSizeProperty, 16);
```

## Style Collections

A **StyleCollection** is a container that holds multiple styles and applies them sequentially to matching elements.

### Style Application Order

Styles are applied in the order they appear in the collection. Later styles can override earlier ones based on property value precedence. Style collections are contained at the application, window, and element levels.

```cpp
// First style
Styles.New().Class("button")
    .Set(Button::BackgroundProperty, Colors::Blue);

// Second style - overrides background if both match
Styles.New().Class("button").Class("primary")
    .Set(Button::BackgroundProperty, Colors::Green);
```

## Value Priority
The class `ValueSource` defines the priority when setting style property values. `Default` has the lowest priority, while `PseudoClass` has the highest.
```c++
enum class ValueSource : uint8_t
{
    Default,
    Inheritance,
    Style,
    Local,
    PseudoClass
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

### ResourceKey

`ResourceKey` provides strongly typed references to theme resources. Using a resource key automatically creates a `ResourceSetter`.

```cpp
style.Set(TextBlock::ForegroundProperty, ResourceKey("TextColor"));
```