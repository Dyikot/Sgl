#define SDL_MAIN_HANDLED

/*
	2. Add UIElement to Scene

		The following example shows a basic setup that includes creating an Application class,
	a Window, and a Scene.
*/

#include "../Sgl/Application.h"
#include "../Sgl/UI/UIElements/TextBlock.h"

using namespace Sgl;

class MyScene: public Scene
{
public:
	MyScene()
	{
		InitializeComponents();
		SetBackground(Colors::WhiteSmoke);
	}
private:
	void InitializeComponents()
	{
		auto text = NewUnique<UIElements::TextBlock>();
		text->SetText("Hello World");
		text->SetFontSize(20);
		text->SetFontStyle(FontStyle::Italic | FontStyle::Underline);
		text->SetPadding(10);
		text->SetForeground(Colors::Black);
		text->SetBackground(Colors::Coral);
		text->SetHorizontalAlignment(HorizontalAlignment::Center);
		text->SetVerticalAlignment(VerticalAlignment::Center);

		SetContent(std::move(text));
	}
};

int main()
{
	Application app;
	app.SetMaxFPS(60);
	app.Window.SetTitle("Test");
	app.Window.SetResizable();
	app.SceneManager.Push<MyScene>();
	app.Run();
}