#define SDL_MAIN_HANDLED

/*
	1. Basic setup

		The following example shows a basic setup that includes creating an Application class,
	a Window, and a Scene.
*/

#include "../Sgl/Application.h"

using namespace Sgl;

class MyScene: public Scene
{
public:
	MyScene() = default;
};

int main()
{
	Application app;

	app.SetMaxFPS(60);
	app.Started += [](Application& sender, const EventArgs& e) { std::cout << "Started\n"; };
	app.Stopped += [](Application& sender, const EventArgs& e) { std::cout << "Stopped\n"; };

	app.Window.SetTitle("Test");
	app.Window.SetWidth(800);
	app.Window.SetHeight(600);
	app.Window.SetDisplayMode(DisplayMode::Window);
	app.Window.SetResizable();

	app.SceneManager.Push<MyScene>();
	app.Run();
}