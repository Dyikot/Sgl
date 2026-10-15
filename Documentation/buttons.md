# Overview
Buttons are the primary way users trigger actions and make selections in Sgl. Every button control derives from `Button` and therefore shares a common core — click event, command support, focus management, and visual states — while each subclass adds its own semantics on top.

## Button
`Button` is the base button control and the most common interactive element in the framework. It performs a single, immediate action when activated.

### Members
| Name | Type | Description |
|---|---|---|
| `Click` | `Event<ButtonEventHandler>` | Raised when the button is activated. |
| `ClickMode` | `ClickMode` | Specifies when a click event is generated. |
| `Command` | `Action<const Any&>` | Command invoked on activation. |
| `CommandParameter` | `Any` | Parameter passed to the command. |

### Example
```cpp
auto button = New<Button>();
button->SetPadding(Thickness(20, 10));
button->SetVerticalAlignment(VerticalAlignment::Center);
button->SetHorizontalAlignment(HorizontalAlignment::Center);
button->SetContent("Click");
button->Click += [](Button& sender, EventArgs e)
{
    Logging::LogDebug("Click");
};
```

## ToggleButton
`ToggleButton` extends `Button` with a persistent binary state. Instead of firing a one-shot action, each activation switches the control between checked and unchecked. It stays "pressed" while checked. The state model it introduces is also the semantic basis for `CheckBox`, `SwitchButton`, and `RadioButton`.

### Members
| Name | Type | Description |
|---|---|---|
| `IsChecked` | `bool` | `true` if checked, `false` if unchecked. |
| `IsCheckedChanged` | `Event<ToggleButtonEventHandler>` | Raised when the state changes. |
| `OnChecked` | `VisualState` | Current toggle state. |

### Example
```cpp
auto button = New<ToggleButton>();
button->SetPadding(Thickness(20, 10));
button->SetVerticalAlignment(VerticalAlignment::Center);
button->SetHorizontalAlignment(HorizontalAlignment::Center);
button->IsCheckedChanged += [](ToggleButton& sender, EventArgs e)
{
    bool checked = sender.IsChecked();
    sender.SetContent(checked ? "Checked" : "Unchecked");
};

button->SetIsChecked(true);
```

## CheckBox and SwitchButton
`CheckBox` and `SwitchButton` simply update their background on visual state change in accordance with `CheckBoxStyle` and `SwitchButtonStyle` styles. For a custom view, simply change its style.

## RadioButton
`RadioButton` lets the user pick exactly one option from a mutually exclusive group. Checking one radio button automatically unchecks the others in the same group. Group membership is defined by property `GroupName`.

### Example
```cpp
const std::string modes[] =
{
    "Light",
    "Dark",
    "System"
};

auto panel = New<StackPanel>();
for(auto& mode : modes)
{
    auto textblock = New<TextBlock>();
    textblock->SetText(mode);
    textblock->SetMinWidth(80);

    auto button = New<RadioButton>();
    button->SetGroupName("Mode");
    button->SetIsChecked(mode == "Light");

    auto layout = New<StackPanel>();
    layout->SetOrientation(Orientation::Horizontal);
    layout->SetSpacing(5);
    layout->Children.Add(textblock);
    layout->Children.Add(button);

    panel->Children.Add(layout);
}
```

## RepeatButton
`RepeatButton` raises `Click` repeatedly while it stays pressed: once after the initial `Delay`, then every `Interval` milliseconds until the user releases it.

### Members
| Name | Type | Description |
|---|---|---|
| `Delay` | `uint32_t` | Time (milliseconds) the button must be held before repeating starts. |
| `Interval` | `uint32_t` | Time (milliseconds) between repeated clicks. |

### Example
```cpp
auto button = New<RepeatButton>();
button->SetWidth(200);
button->SetHeight(100);
button->SetVerticalAlignment(VerticalAlignment::Center);
button->SetHorizontalAlignment(HorizontalAlignment::Center);
button->SetVerticalContentAlignment(VerticalAlignment::Center);
button->SetHorizontalContentAlignment(HorizontalAlignment::Center);
button->SetContent("Press");
button->Click += [clicks = 0](Button& sender, EventArgs e) mutable
{
    clicks++;
    sender.SetContent(std::to_string(clicks));
};
```