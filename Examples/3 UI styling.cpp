#define SDL_MAIN_HANDLED

/*
	3. UI styling

		The following example shows a TextBlock styling. To do this, you need to create styles 
	and add them to Styles. Styles can be added to the Application, Scene or UI element levels. 
		When creating a style, you specify the target element type and select the necessary properties
	and specify the values of the corresponding type. In order for the styles to be applied to 
	an element, you need to specify them in the Classes of the corresponding element.
*/

#include "../Sgl/Application.h"
#include "../Sgl/UI/UIElements/TextBlock.h"

using namespace Sgl;
using namespace Sgl::UIElements;

class MyScene: public Scene
{
public:
	MyScene()
	{
		InitializeComponents();
		InitializeStyles();
		SetBackground(Colors::WhiteSmoke);
	}
private:
	void InitializeComponents()
	{
		auto text = New<TextBlock>();
		text->Classes.Set("centerAlignment h2");
		text->SetText("Hello World");
		text->SetFontStyle(FontStyle::Italic | FontStyle::Underline);
		text->SetPadding(10);
		text->SetForeground(Colors::Black);
		text->SetBackground(Colors::Coral);

		SetContent(text);
	}

	void InitializeStyles()
	{
		Styles.Add<Layoutable>("centerAlignment")
			.With(Layoutable::VerticalAlignmentProperty, VerticalAlignment::Center)
			.With(Layoutable::HorizontalAlignmentProperty, HorizontalAlignment::Center);
	}
};

int main()
{
	Application app;
	app.Styles.Add<TextBlock>("h2")
		.With(TextBlock::FontSizeProperty, 24);
	app.SetMaxFPS(60);
	app.Window.SetTitle("Test");
	app.SceneManager.Push<MyScene>();
	app.Run();
}