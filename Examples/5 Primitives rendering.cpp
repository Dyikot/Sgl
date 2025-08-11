#define SDL_MAIN_HANDLED

/*
	5. Primitives rendering

		This example examines rendering capabilities. Rendering is performed in the Render method 
	of the scene and all classes implementing the IRenderable interface. This method has 
	a RenderContext argument that allows rendering of primitives: point, line, rectangle,
	circle, textures and arbitrary figures.
*/

#include "../Sgl/Application.h"

using namespace Sgl;
using namespace Sgl::UIElements;

class MyScene: public Scene
{
private:
	Texture _homeTexture = Texture("home.png");
public:
	MyScene()
	{
		SetBackground(Colors::WhiteSmoke);
	}
private:
	void Render(RenderContext context) override
	{
		Scene::Render(context);

		auto rect = FRect { .x = 100, .y = 100, .w = 50, .h = 50 };
		context.DrawRectange(rect, Colors::Blue);

		auto fillRect = FRect(200, 100, 50, 50);
		context.DrawFillRectangle(fillRect, Colors::DarkCyan);

		auto circlePosition = FPoint(300, 100);
		int diameter = 50;
		context.DrawCircle(circlePosition, diameter, Colors::Red);

		auto homeTextureTarget = FRect(400, 100, 48, 48);
		context.DrawTexture(_homeTexture, homeTextureTarget);
	}
};

int main()
{
	Application app;
	app.SetMaxFPS(60);
	app.Window.SetTitle("Test");
	app.SceneManager.Push<MyScene>();
	app.Run();
}