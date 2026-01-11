## About
**Sgl** is a cross-platform UI framework developed on top of C++ 20 and SDL3.
The project is currently in development. The following has been implemented so far:
- Layout system
- Basic styling, ThemeMode
- Basic UIElements: Border, Image, TextBlock, Button, StackPanel, Canvas
- Data binding
- Localization
- Threading: Timer, ThreadPool, async operations using c++ 20 coroutines

## Roadmap
- Pseudo classes
- Focus manager
- Animation
- Drag & drop
- Expanding UIElements base
- New renderer
- Notifications
- Documentation

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
	```
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