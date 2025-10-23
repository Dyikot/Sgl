#define SDL_MAIN_HANDLED

/*
	4. Data binding

		In this example, data binding is considered. As an example, a SceneViewModel scene model
	was created. Let's add the tracked property "Name". After that, we will add the data context 
	of our scene model. And after that, we will add a one-way binding with the "Text" property
	of the text block. 
		The Bind method also accepts a third argument - BindingMode. This method also has other 
	overloads that allow you to bind OneWayToSource model fields without the need to create 
	properties and inherit from ObservableObject.
		Also, as an alternative to the OneWay and TwoWay bindings, you can use the GetOberver 
	method and add it to the model for the required setter.
*/

#include "../Sgl/Application.h"
#include "../Sgl/UI/UIElements/TextBlock.h"

using namespace Sgl;
using namespace Sgl::UIElements;

class SceneViewModel: public ObservableObject
{
private:
	std::string _name;
public:
	SceneViewModel(std::string name) : _name(std::move(name)) {}

	void SetName(const std::string& value)
	{
		SetProperty(NameProperty, _name, value);
	}

	const std::string& GetName() const
	{
		return _name;
	}
public:
	static inline ObservableProperty<SceneViewModel, const std::string&> NameProperty =
		ObservableProperty<SceneViewModel, const std::string&>(&SetName, &GetName);
};

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
		auto text = New<TextBlock>();
		text->SetDataContext(New<SceneViewModel>("Alex"));
		text->Bind(TextBlock::TextProperty, SceneViewModel::NameProperty);

		SetContent(text);
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