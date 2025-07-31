#pragma once

#include "../UI/UIElement.h"
#include "../Render/IRenderable.h"
#include "../Base/Time/TimeSpan.h"
#include "../Input/TextEventArgs.h"

namespace Sgl
{	
	class Scene: public IRenderable, public IStyleProvider
	{
	public:
		struct Background
		{
			Color Color;
			Shared<Texture> Texture;
		};

		ResourcesMap Resources;
		StyleMap Styles;
	protected:
		Shared<UIElement> _content;
		IStyleProvider* _stylingParent;
	private:
		Background _background;
		std::reference_wrapper<const Cursor> _cursor;
	public:
		Scene();
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		virtual ~Scene() = default;

		void SetBackground(Color value);
		void SetBackground(Shared<Texture> value);
		void SetBackground(Color color, Shared<Texture> texture);
		Background GetBackground() const { return _background; }

		void SetCursor(const Cursor& value);
		const Cursor& GetCursor() const { return _cursor; }

		void SetContent(Shared<UIElement> value);
		Shared<UIElement> GetContent() const { return _content; }

		StyleMap& GetStyles() override { return Styles; }
		IStyleProvider* GetStylingParent() { return _stylingParent; }

		void Render(RenderContext context) const override;
		virtual void Process(TimeSpan elapsed);
	protected:
		virtual void OnResumed() {}
		virtual void OnStopped() {}
		virtual void OnCreated();
		virtual void OnDestroying() {}
		virtual void OnKeyUp(const KeyEventArgs& e); 
		virtual void OnKeyDown(const KeyEventArgs& e); 
		virtual void OnMouseMove(const MouseEventArgs& e); 
		virtual void OnMouseDown(const MouseButtonEventArgs& e); 
		virtual void OnMouseUp(const MouseButtonEventArgs& e); 
		virtual void OnMouseWheelChanged(const MouseWheelEventArgs& e);
		virtual void OnTextInput(const TextInputEventArgs& e);
		virtual void OnTextEditing(const TextEditingEventArgs& e);
	private:
		void UpdateInvalidatedLayoutAndStyle();

		friend class Application;
		friend class SceneManager;
	};
}