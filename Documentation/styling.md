# Styling

## What is Style?

A **Style** is a reusable set of property values that can be applied to UI elements. Styles separate visual appearance from element logic, enabling consistent theming and easier maintenance.

### Style Structure

A `Style` consists of:
- **Selector**: Determines which elements the style applies to
- **Projection** (optional): Specifies which part of a composite element to style
- **Setters**: A collection of property-value pairs to apply

### Setters

Setters are the mechanism by which styles apply values to properties. There are two types:

1. **Setter**: Applies a fixed value directly
2. **ResourceSetter**: Resolves a value from theme resources at runtime

```cpp
// Create a style for all Button elements
Styles.Add(Selector().OfType<Button>())
    // Set a fixed value
    .Set(Button::MarginProperty, Thickness(5))
    // Set a themed resource (resolved at runtime)
    .Set(Button::BackgroundProperty, ResourceKey("ButtonBgColor"));
```

## Selector

Selectors determine which elements a style applies to. The `Selector` class provides a fluent API for building complex matching rules.

### Supported Selector Types

#### Type Selector (`.OfType<T>()`)

Matches elements of the exact specified type.

```cpp
// Matches only TextBlock elements
Selector().OfType<TextBlock>()
```

#### Type Identity Selector (`.Is<T>()`)

Matches elements of the specified type or any derived type.

```cpp
// Matches Button and all derived types (e.g., ToggleButton)
Selector().Is<Button>()
```

#### Name Selector (`.Name()`)

Matches elements by their `Name` property.

```cpp
// Matches element with Name == "SubmitButton"
Selector().Name("SubmitButton")
```

#### Class Selector (`.Class()`)

Matches elements by CSS-like class names. Multiple classes can be specified; all must match.

```cpp
// Matches elements with class "primary"
Selector().Class("primary")

// Matches elements with both "primary" AND "large" classes
Selector().Class("primary").Class("large")

// Usage: Set classes on element
element->SetClasses("primary large");
```

#### Pseudo-Class Selector (`.On()`)

Matches elements in a specific state. Pseudo-classes are registered by name and identified by `PseudoClassId`.

```cpp
// Matches elements in the "hover" state
Selector().On("hover")

// Matches elements in multiple states (all must be active)
Selector().On("hover").On("pressed")
```

Supported pseudo-classes:
- `hover` - Mouse is over the element
- `pressed` - Mouse button is pressed on the element
- `checked` - Toggle state is active

#### Predicate Selector (`.Where()`)

Matches elements using custom logic via a predicate function.

```cpp
// Matches elements with a specific property value
Selector().Is<Button>().Where([](StyleableElement& element)
{
    auto& button = static_cast<Button&>(element);
    return button->GetClickMode() == ClickMode::Press;
})
```

### Combining Selectors

Selectors can be combined to create more specific rules. All conditions must match (AND logic).

```cpp
// Matches ToggleButton elements with class "toggle" and name "ThemeToggle"
Selector()
    .OfType<ToggleButton>()
    .Class("toggle")
    .Name("ThemeToggle")

// Matches Button elements in the pressed state
Selector()
    .Is<Button>()
    .Class("primary")
    .On("pressed")
```

### Selector Matching

Selectors use two matching methods:
- **`Match()`**: Evaluates type, name, class, and predicate conditions
- **`MatchState()`**: Evaluates pseudo-class conditions
- **`HasState()`**: Returns true if the selector has pseudo-class conditions

## Projection

Projections allow styles to target specific parts of composite elements. Instead of styling the element itself, a projection redirects the style application to a child or internal part.

### TargetProjection

A `TargetProjection` is a callable that takes a `StyleableElement&` and returns a `StyleableElement&` to style.

Built-in projections:
- `ContentUIElement_Content`
- `Window_Content`
- `Panel_FirstChild`
- `Panel_LastChild`
- `Panel_NthChild`

### Composed Projections

Projections can be chained using the `>` operator or `ComposedProjection` class.

```cpp
Styles.Add(Selector().OfType<CustomControl>(), 
           Panel_FirstChild() > ContentUIElement_Content());
```

## Style Collections

A **StyleCollection** is a container that holds multiple styles and applies them sequentially to matching elements.

### Style Application Order

Styles are applied in the order they appear in the collection. Later styles can override earlier ones based on property value precedence.

```cpp
// First style
styles.Add(Selector().Class("button"))
    .Set(Button::BackgroundProperty, Colors::Blue);

// Second style - overrides background if both match
styles.Add(Selector().Class("button").Class("primary"))
    .Set(Button::BackgroundProperty, Colors::Green);
```

### IStyleHost Interface

`IStyleHost` defines an interface for objects that can host style collections:

```cpp
class IStyleHost
{
public:
    virtual StyleCollection& GetStyles() = 0;
    virtual IStyleHost* GetStylingParent() = 0;
};
```

### StyleableElement

`StyleableElement` is the base class for all styleable UI elements:

```cpp
class StyleableElement : public BindableObject, public IStyleHost
{
public:
    std::string Name;
    StyleCollection Styles;
    PseudoClassesSet PseudoClasses;

    void SetClasses(std::string_view classNames);
    const std::vector<std::string>& GetClasses() const;

    void ApplyStyle();
};
```

## Theme Styling

Theme styling enables UI elements to automatically adapt their appearance based on the active theme (Light or Dark). This is achieved through theme-aware resources.

### ThemeResourceProvider

`ThemeResourceProvider` manages themed color and brush resources:

```cpp
#include "Sgl/Base/Media/ThemeResourceProvider.h"

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

### ThemeMode and ThemeVariant

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

### Switching Themes

```cpp
// Set current theme
app.SetThemeVariant(ThemeVariant::Dark);

// Retrieve themed resources (automatically uses current theme)
Brush bgBrush = app.Resources.GetBrush("BackgroundBrush");
```

### ResourceKey

`ResourceKey` provides strongly-typed references to theme resources:

```cpp
// Use in style setter
style.Set(TextBlock::ForegroundProperty, ResourceKey("TextColor"));
```