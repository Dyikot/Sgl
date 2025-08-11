# Sgl - Simple game library

## About
**Sgl (Simple game library)** is a lightweight, modern C++ library built on top of [SDL2](https://www.libsdl.org/) to streamline 2D game development. Designed with simplicity and extensibility in mind, Sgl provides a clean, high-level API that abstracts away low-level boilerplate while maintaining performance and control.

## Key Features
- **Application Framework**: 
  - Simple `Application` class for game lifecycle management
  - Window management with `Window` class
  - Built-in game loop with frame rate control
- **Rendering**: 
  - 2D rendering with `RenderContext`
  - Texture loading and management
- **Event System**: 
  - C#-style event system using `Event<T>` and `Delegate<T>` template classes.
  - Input event handling (keyboard/mouse)
- **Scene Management**: 
  - Stack-based `SceneManager` for scene transitions (e.g., menus, levels, pause screens)
- **Styling System**: 
  - Define and apply styles to any `StyleableElement` using class names—inspired by CSS
  - Promotes reusable, themeable UI design with minimal code
- **User Interface**
  - Hierarchical UI system built on `UIElement`,`ContentUIElement`, and `Panel`
  - Easy creation of custom UI components through inheritance
  - Supports nesting, layout composition, and styling via class names
  - Fully integrates with the styling system for consistent visual design
- **Data Binding**
  - Robust and type-safe data binding via `ObservableProperty`, `ObservableObject` and `BindableObject`
  - Supports **OneWay**, **TwoWay**, and **OneWayToSource** binding modes
  - Bind UI elements directly to fields or setter methods in view models
  - Zero-overhead abstraction with compile-time type checking
  - Enables clean MVVM-style patterns for robust UI architecture
- **Time Utilities**: 
  - `Timer`: schedule callbacks after a duration
  - `Stopwatch`: high-precision time measurement for profiling and gameplay logic
- **Audio System**: 
  - `SoundChunk` for short sound effects
  - `Music` for streaming audio
  - `Playlist` for managing background music
- **Utilities**
  - `Any` – type-safe container for heterogeneous values
  - `Lazy<T>` – deferred initialization
  - `Ranges` – composable range utilities inspired by C++20 ranges and views
  - `TimeSpan` – a representation of time duration inspired by the C# implementation
  - and more

## Dependencies
- SDL2 ([2.28.5](https://github.com/libsdl-org/SDL/releases/tag/release-2.28.5) or newer)
- [SDL_image](https://github.com/libsdl-org/SDL_image) (for texture loading)
- [SDL_mixer](https://github.com/libsdl-org/SDL_mixer) (for music/sound)
- [Random](https://github.com/Dyikot/Random) library
- C++20 compliant compiler
