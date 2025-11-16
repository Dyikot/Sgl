## About
**Sgl** is a modern, cross-platform UI framework built from the ground up with C++ 20 and [SDL3](https://www.libsdl.org/).
Designed for performance-critical applications, Sgl combines the power of modern C++ with the reliability of SDL3 to provide
a robust foundation for native applications. Inspired by WPF and Avalonia patterns, 
it brings familiar declarative UI concepts to the C++ ecosystem with full hardware acceleration and platform-native performance.

## Key Features
- **Application Framework**: 
  - Simple `Application` class for game lifecycle management
  - Window management with `Window` class
- **Rendering**: 
  - 2D rendering with `RenderContext`
  - Texture loading and management
- **Event System**: 
  - C#-style event system using `Event<T>` and `Delegate<T>` template classes.
  - Input event handling (keyboard/mouse)
- **Styling System**: 
  - Define and apply styles to any `StyleableElement` using class names—inspired by CSS
  - Promotes reusable, themeable UI design with minimal code
- **User Interface**
  - Hierarchical UI system built on `UIElement`,`ContentUIElement`, and `Panel`
  - Easy creation of custom UI components through inheritance
  - Supports nesting, layout composition, and styling via class names
  - Fully integrates with the styling system for consistent visual design
- **Data Binding**
  - Robust and type-safe data binding via `ObservableProperty` and `ObservableObject`
  - Supports **OneWay**, **TwoWay**, and **OneWayToSource** binding modes
  - Bind UI elements directly to fields or setter methods in view models
  - Zero-overhead abstraction with compile-time type checking
  - Enables clean MVVM-style patterns for robust UI architecture
- **Time Utilities**: 
  - `Timer`: schedule callbacks after a duration
  - `Stopwatch`: high-precision time measurement for profiling and gameplay logic
- **Utilities**
  - `Any` – type-safe container for heterogeneous values
  - `Lazy<T>` – deferred initialization
  - `TimeSpan` – a representation of time duration inspired by the C# implementation
  - and more

## Current state
The framework is still in the early stages of development.

## Dependencies
- [SDL3](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.26)
- [SDL_image](https://github.com/libsdl-org/SDL_image/releases/tag/release-3.2.4)
- [SDL_ttf](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-3.2.2)
- C++20 compliant compiler

## Useful libraries
The following libraries may be useful when developing applications with user interfaces:
- [Random](https://github.com/Dyikot/Random) - header-only C++20 random library based on <random> with range support and thread-safe operations
- [Ranges](https://github.com/Dyikot/Ranges) - range library based on std::ranges and inspired by C# LINQ