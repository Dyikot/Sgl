## About
**Sgl** is a cross-platform UI framework developed on top of C++ 20 and SDL3.
The project is currently in development. 

## Features
- **Layout system** - Measure and arrange UI elements
- **Styling** - Style, selectors, pseudo-classes, projections
- **Theming** - Light/Dark theme modes
- **Data binding** - MVVM support
- **Localization** - Runtime language switching with CSV-based localization files
- **Threading** - `Timer`, `ThreadPool`, `Dispatcher`, async operations using C++20 coroutines
- **UI Elements**:
  - Panels: `StackPanel`, `WrapPanel`, `DockPanel`, `Grid`, `UniformGrid`, `Canvas`
  - Basic: `Border`, `TextBlock`, `Image`
  - Buttons: `Button`, `ToggleButton`, `CheckBox`, `RadioButton`

## Roadmap
- New UIElements
- Focus manager
- Animation
- New renderer
- Declarative UI (XML Compiler)
- Notifications
- Drag & drop
- Documentation

## Quick Start
See the [Getting Started Guide](Documentation/getting-started.md) for detailed prerequisites, dependency setup, and build instructions.

## Documentation
- [Getting Started](Documentation/getting-started.md)
- [Events](Documentation/events.md)
- [Styling](Documentation/styling.md)
- [Data Binding](Documentation/data-binding.md)