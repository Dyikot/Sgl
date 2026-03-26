## About
**Sgl** is a cross-platform UI framework developed on top of C++ 20 and SDL3.
The project is currently in development. The following has been implemented so far:
- **Layout system** - Measure and arrange UI elements
- **Styling system** - Style, selectors, pseudo-classes, projections and dynamic theme resources
- **Theme support** - Light/Dark theme switching with runtime resource dictionaries
- **UI Elements**:
  - Content: `Border`, `TextBlock`, `Image`, `Button`
  - Panels: `StackPanel`, `Canvas`
- **Data binding** - `INotifyPropertyChanged`, `ObservableObject`, data templates
- **Localization** - Runtime language switching with CSV-based localization files
- **Threading** - Timer, ThreadPool, Dispatcher, async operations using C++20 coroutines

## Roadmap
- New Panels
- Focus manager
- Animation
- New UIElements
- Drag & drop
- New renderer
- Notifications
- Documentation

## Features

### Styling
Sgl provides a powerful CSS-like styling system for UI elements.
Styles are defined using selectors and setters.

```cpp
// Style all TextBlock elements
Styles.Add(Selector().OfType<TextBlock>())
    .Set(TextBlock::FontSizeProperty, 16.0f)
    .Set(TextBlock::ForegroundProperty, Colors::Black);

// Style buttons when hovered
Styles.Add(Selector().Is<Button>().On("hover"))
    .Set(Button::BackgroundProperty, Colors::Green);
```

Use theme resources to automatically switch colors/brushes based on the active theme:
```cpp
// Register themed resources during app initialization
app.Resources.AddBrush("Background", Colors::White, Colors::Black);

// Use resources in styles
Styles.Add(Selector().Is<Renderable>())
    .Set(Renderable::BackgroundProperty, ResourceKey("Background"));
```

Projections allow styling specific parts of composite elements:

```cpp
// Style the content area of a Border
Styles.Add(Selector().OfType<Border>(), ContentUIElement_Content())
    .Set(TextBlock::ForegroundProperty, Colors::Red);
```

### Data Binding

Create an observable and bind it to UI element:

```cpp
// ViewModel with property change notification
class PersonViewModel : public ObservableObject
{
public:
    void SetName(const std::string& value) { SetProperty(NameProperty, _name, value); }
    const std::string& GetName() const { return _name; }
    
    static inline DirectProperty NameProperty { &SetName, &GetName };
private:
    std::string _name;
};

// Create
auto vm = New<PersonViewModel>();
vm->SetName("John");

// Bind
auto textBlock = New<TextBlock>();
textBlock->SetDataContext(vm);
textblock->Bind(TextBlock::TextProperty, PersonViewModel::NameProperty);
```

## Dependencies
- [SDL3](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.26)
- [SDL_image](https://github.com/libsdl-org/SDL_image/releases/tag/release-3.2.4)
- [SDL_ttf](https://github.com/libsdl-org/SDL_ttf/releases/tag/release-3.2.2)

## Installation
To install, you need to do the following steps:
1.  Clone or download the Sgl source code.
2.  Install SDL3 dependencies and [Premake](https://premake.github.io/download/).
3.  Create **Dependencies** directory. And create two more directories in it: **Include** and **Libraries**.
4.  Add the SDL3, SDL_image, and SDL_ttf static libraries to **Libraries**. And thier headers (SDL3/.., SDL_image/.., SDL_ttf/..) to **Include**.
5.  Run in console the premake5.lua script. Learn more about command [argumnets](https://premake.github.io/docs/Using-Premake).
	``` 
	premake5.lua [action]
	```
	
6.  After that, two projects will be created: Sgl and App. Add the main file to the App project. Make sure that SDL3/SDL_main.h is included.
	```cpp
	#include <SDL3/SDL_main.h>
	#include <Application.h>
	
	int main(int argc, char* argv[])
	{
		Sgl::Application app;
		app.MainWindow = New<Sgl::Window>();
		app.Run();

		return 0;
	}
	```
7.	Build the project.
8.	Add dynamic link libraries (SDL3.dll, SDL_image.dll, SDL_ttf.dll) to output directory. By default: bin/App/Output/{Debug/Release}.
9.	Run the project.