## About
Sgl brings the retained-mode UI model popularized by WPF to modern C++ — without heavyweight runtimes or vendor lock-in. It combines the performance of native code with the productivity features of modern UI frameworks: panels, styles, theming, data binding, and localization — all rendered by the lightweight SDL3 backend.

**Note:** Sgl is under active development. The API is not stable yet and may change between releases.

## Features
- **Layout system** - measure and arrange UI elements
- **Input** - input and focus managers
- **Styling** - style, selectors, pseudo-classes
- **Theming** - build-in Light/Dark theme modes
- **Data binding** - MVVM support
- **Localization** - runtime language switching with CSV-based localization files
- **Threading** - `Timer`, `ThreadPool`, `Dispatcher`, async operations using C++20 coroutines
- **UI elements**:
  - *Panels*: `Panel`, `StackPanel`, `WrapPanel`, `DockPanel`, `Grid`, `UniformGrid`, `Canvas`
  - *Buttons*: `Button`, `ToggleButton`, `CheckBox`, `SwitchButton`, `RadioButton`, `RepeatButton`
  - *Text*: `TextBlock`
  - *Containers*: `Border`
  - *Media*: `Image`
  - *RangeBase*: `ProgressBar`, `Slider`, `ScrollBar`

## Roadmap
- New UI elements
- Animation
- Notifications
- Drag & drop
- New renderer
- Documentation

## Quick Start
See the [Getting Started Guide](Documentation/getting-started.md) for detailed prerequisites, dependency setup, and build instructions.

## Documentation
- [Getting Started](Documentation/getting-started.md)
- [Events](Documentation/events.md)
- [Styling](Documentation/styling.md)
- [Panels](Documentation/panels.md)
- [Data Binding](Documentation/data-binding.md)