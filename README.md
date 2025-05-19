# Sgl - Simple game library

> A lightweight 2D game development library built on top of SDL2

## About
**Sgl** is a minimalistic C++ library designed to simplify 2D game development using [SDL2](https://www.libsdl.org/).

**Key Features:**
- Simple Application and Window creation/managment
- 2D rendering using RenderContext
- C# inspired Events
- Scenes and SceneManager
- Stylization UIElements and Scenes
- Includes time utilities: Timer and Stopwatch
- Audio wrappers: SoundChunk, Music and Playlist

Perfect for beginners learning game development and experienced developers who want to avoid boilerplate SDL2 setup.

## Example
This example shows how to create an application, set up a window, and style the scene.
```
void SceneSetter(Sgl::StyleProperties<IVisual>& p)
{
	p.BackgroundColor = Colors::Green;
}

class MyScene: public Sgl::Scene
{
public:
	MyScene()
	{
		ClassStyle.Use<SceneSetter>();
	}

	void OnProcessing(TimeSpan elapsed) override
	{
	
	}
};

int main()
{	
	Sgl::Application app;
	app.SetMaxFrameRate(60);
	app.Window.SetTitle("Test");
	app.Window.EnableResizable();
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
