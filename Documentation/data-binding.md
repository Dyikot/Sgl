# Data Binding

## What is Data Binding?

Data binding is a mechanism that establishes a connection between a **source** (typically a data object or view model) and a **target** (typically a UI element). This connection allows data to flow automatically between the source and target without manual synchronization.

### Source and Target

- **Source**: The object that provides the data. In the Sgl framework, the source typically implements the `INotifyPropertyChanged` interface, which allows it to notify subscribers when its properties change.

- **Target**: The object that consumes the data. Targets are usually UI elements that inherit from `BindableObject`, which provides the infrastructure for receiving and applying data bindings.

### How It Works

When a binding is established:
1. The target property is initialized with the current value from the source
2. The target subscribes to the source's `PropertyChanged` event
3. When the source property changes, the new value is automatically propagated to the target
4. For two-way bindings, changes on the target are also propagated back to the source

This automatic synchronization eliminates boilerplate code for updating UI elements when data changes.

**Example - Basic Binding:**
```cpp
// Bind a TextBlock's text to a view model property
textBlock->Bind(
    TextBlock::TextProperty,
    ViewModel::TitleProperty,
    BindingMode::OneWay
);
```

---

## Data Context

The **Data Context** is the default source object for bindings. It is set on a `BindableObject` using the `SetDataContext` method and can be inherited by child elements in the UI tree.

### Setting Data Context

```cpp
#include "Sgl/Base/Ref.h"
#include "Sgl/Data/BindableObject.h"

// Create a view model wrapped in a Ref
auto viewModel = New<MyViewModel>();

// Set it as the data context
myElement->SetDataContext(viewModel);
```

### Data Context Inheritance

When a `UIElement` is added as a child to a parent, it automatically inherits the parent's data context:

```cpp
// Parent has a data context
parent->SetDataContext(viewModel);

// Child automatically inherits the data context
parent->Children.Add(child);  // child->GetDataContext() == viewModel
```

### Accessing Data Context

```cpp
// Get the current data context
auto context = bindableObject->GetDataContext();

// Cast to specific type
auto viewModel = context.GetAs<MyViewModel>();
```

---

## Binding Mode

The `BindingMode` enum controls the direction of data flow in a binding:

```cpp
enum class BindingMode
{
    OneWay,         // Source → Target
    OneWayToSource, // Target → Source
    TwoWay          // Source ↔ Target (bidirectional)
};
```

### OneWay

Data flows from source to target only. Changes to the source update the target, but changes to the target do not affect the source. This is the default and most common mode, suitable for read-only displays.

**Example:**
```cpp
// Binding a TextBlock to display a title
textBlock->Bind(
    TextBlock::TextProperty,
    ViewModel::TitleProperty,
    BindingMode::OneWay
);
```

### OneWayToSource

Data flows from target to source only. Changes to the target update the source, but changes to the source do not update the target. Useful for capturing user input without displaying initial values.

**Example:**
```cpp
// Binding a TextBox to capture user input
textBox->Bind(
    TextBox::TextProperty,
    ViewModel::SearchQueryProperty,
    BindingMode::OneWayToSource
);
```

### TwoWay

Data flows in both directions. Changes on either side are synchronized. Ideal for interactive controls like text boxes, toggle buttons, and sliders.

**Example:**
```cpp
// Binding a ToggleButton to a boolean setting
toggleButton->Bind(
    ToggleButton::IsCheckedProperty,
    ViewModel::EnableNotificationsProperty,
    BindingMode::TwoWay
);
```

---

## Converters

Converters transform values between source and target during binding. They are useful when the source and target types differ or when you need to apply formatting or logic to the value.

### Built-in Converters

The framework provides several built-in converters in the `Converters` namespace:

| Converter | Description |
|-----------|-------------|
| `IntToStringConverter` | Converts `int` ↔ `std::string` |
| `UIntToStringConverter` | Converts `uint32_t` ↔ `std::string` |
| `Int64ToStringConverter` | Converts `int64_t` ↔ `std::string` |
| `UInt64ToStringConverter` | Converts `uint64_t` ↔ `std::string` |
| `FloatToStringConverter` | Converts `float` ↔ `std::string` |
| `DoubleToStringConverter` | Converts `double` ↔ `std::string` |
| `InverseBoolConverter` | Converts `bool` → `bool` (inverted) |

### Using Converters in Bindings

```cpp
#include "Sgl/Data/Converters.h"

// Display an integer counter as text
textBlock->Bind(
    TextBlock::TextProperty,
    ViewModel::CounterProperty,
    Converters::IntToStringConverter(),
    BindingMode::OneWay
);

// Invert a boolean value
toggleButton->Bind(
    ToggleButton::IsCheckedProperty,
    ViewModel::IsDisabledProperty,
    Converters::InverseBoolConverter(),
    BindingMode::OneWay
);
```

### Custom Converters

Create a custom converter by defining a callable type with `operator()` for the conversion direction(s) you need:

```cpp
// Converter for displaying a score as a formatted string
struct ScoreToStringConverter
{
    std::string operator()(int score) const
    {
        return std::format("Scope: {}");
    }
};

// Usage
textBlock->Bind(
    TextBlock::TextProperty,
    ViewModel::ScoreProperty,
    ScoreToStringConverter(),
    BindingMode::OneWay
);
```

### Converter with Fallback

Some converters support fallback values for invalid conversions:

```cpp
// Use fallback value of -1 when parsing fails
textBlock->Bind(
    TextBlock::TextProperty,
    ViewModel::ValueProperty,
    Converters::IntToStringConverter(-1),
    BindingMode::TwoWay
);
```

---

## Data Templates

Data templates define how data objects are visualized as UI elements. They enable automatic UI generation based on the type of the data object, following the MVVM pattern.

### IDataTemplate Interface

```cpp
class IDataTemplate
{
public:
    virtual Ref<UIElement> Build(const Ref<INotifyPropertyChanged>& data) = 0;
    virtual bool Match(const Ref<INotifyPropertyChanged>& data) const = 0;
};
```

- **Build**: Creates and returns a UI element for the given data object
- **Match**: Determines whether this template can handle the specified data object

### Creating a Data Template

```cpp
#include "Sgl/Data/IDataTemplate.h"
#include "Sgl/UIElements/TextBlock.h"

class PersonViewModel : public ObservableObject
{
public:
    void SetName(const std::string& value) { SetProperty(NameProperty, _name, value); }
    const std::string& GetName() const { return _name; }
    
    void SetAge(int value) { SetProperty(AgeProperty, _age, value); }
    int GetAge() const { return _age; }

    static inline DirectProperty NameProperty { &SetName, &GetName };
    static inline DirectProperty AgeProperty { &SetAge, &GetAge };
private:
    std::string _name;
    int _age;
};

// Data template for PersonViewModel
class PersonDataTemplate : public IDataTemplate
{
public:
    Ref<UIElement> Build(const Ref<INotifyPropertyChanged>& data) override
    {
        auto panel = New<StackPanel>();
        panel->SetOrientation(Orientation::Horizontal);

        // Name label
        auto nameLabel = New<TextBlock>();
        nameLabel->SetDataContext(data);
        nameLabel->Bind(TextBlock::TextProperty, PersonViewModel::NameProperty);

        // Age label with converter
        auto ageLabel = New<TextBlock>();
        ageLabel->SetDataContext(data);
        ageLabel->Bind(
            TextBlock::TextProperty,
            PersonViewModel::AgeProperty,
            Converters::IntToStringConverter()
        );

        panel->Children.Add(nameLabel);
        panel->Children.Add(ageLabel);

        return panel;
    }

    bool Match(const Ref<INotifyPropertyChanged>& data) const override
    {
        return data.Is<PersonViewModel>();
    }
};
```

### Using Data Templates

```cpp
// Create view model
auto person = New<PersonViewModel>();
person->SetName("John Doe");
person->SetAge(30);

// The template automatically creates a content presenter for the given data type
auto element = New<ContentUIElement>();
element->SetContent(person);
element->SetContentTemplate(New<PersonDataTemplate>());
```