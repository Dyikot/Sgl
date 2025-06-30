# Sgl - Simple game library

> A lightweight 2D game development library built on top of SDL2

## About
**Sgl** is a minimalistic C++ library designed to simplify 2D game development using [SDL2](https://www.libsdl.org/).

## Key Features
- **Application Framework**: 
  - Simple `Application` class for game lifecycle management
  - Window management with `Window` class
  - Built-in game loop with frame rate control
- **Rendering**: 
  - 2D rendering with `RenderContext`
  - Texture loading and management
- **Event System**: 
  - C#-style events with delegates and observers
  - Input event handling (keyboard/mouse)
- **Scene Management**: 
  - Stack-based `SceneManager` for scene transitions
- **Styling System**: 
  - Declarative UI element styling with `StyleProperties`
  - Scene-level style configuration
- **Time Utilities**: 
  - `Timer` with duration/interval callbacks
  - `Stopwatch` for precise time measurements
- **Audio System**: 
  - `SoundChunk` for short sound effects
  - `Music` for streaming audio
  - `Playlist` for managing background music

Perfect for beginners learning game development and experienced developers who want to avoid boilerplate SDL2 setup.

## Example
This example shows how to create an application, set up a window, and style the scene.
```
#include "../Sgl/Application.h"
#include "../Sgl/UI/UIElement/UIElement.h"

class MyScene: public Scene
{
public:
	MyScene()
	{
		BackgroundColor = Colors::Whitesmoke;

		auto style = Style<Button>::New(Selector { .Class = "test" });
		style->Setters.Add(&Button::BackgroundColor, Colors::Grey);
		style->Setters.Add(&Button::Cursor, Cursors::Arrow);
		style->Setters.Add(&Button::Width, 150);
		style->Setters.Add(&Button::Height, 100);

		auto button = Button::New();
		button->Classes = { "test" };
		button->Styles.Add(style);

		Layout = Canvas::New();
		Layout->Children.Add(button);
	}

	void OnRender(RenderContext rc) const override
	{
		Scene::OnRender(rc);
	}

	void Process(TimeSpan elapsed) override
	{
		Scene::Process(elapsed);
	}
};

int main()
{	
	Application app;
	app.SetMaxFrameRate(60);
	app.Window.SetTitle("Test");
	app.SceneManager.Push<MyScene>();
	app.Run();
}
```

## Dependencies
- SDL2 ([2.28.5](https://github.com/libsdl-org/SDL/releases/tag/release-2.28.5) or newer)
- [SDL_image](https://github.com/libsdl-org/SDL_image) (for texture loading)
- [SDL_mixer](https://github.com/libsdl-org/SDL_mixer) (for music/sound)
- [Random](https://github.com/Dyikot/Random) library
- C++20 compliant compiler
