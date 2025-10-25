#pragma once

#include "../UI/UIElement.h"
#include "../Base/Time/TimeSpan.h"
#include "../Input/TextEventArgs.h"
#include "../Base/Ref.h"

namespace Sgl
{	
	class Scene: public Renderable
	{
	protected:
		Ref<UIElement> _content;
	public:
		Scene();
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		virtual ~Scene() = default;

		void SetContent(Ref<UIElement> value);
		Ref<UIElement> GetContent() const { return _content; }

		void Render(RenderContext context) override;
		virtual void Process(TimeSpan elapsed);
	protected:
		void OnCursorChanged(const Cursor& cursor) override;
		virtual void OnResumed() {}
		virtual void OnStopped() {}
		virtual void OnCreated();
		virtual void OnDestroying() {}
		virtual void OnKeyUp(KeyEventArgs& e) {}
		virtual void OnKeyDown(KeyEventArgs& e) {}
		virtual void OnMouseMove(MouseEventArgs& e); 
		virtual void OnMouseDown(MouseButtonEventArgs& e); 
		virtual void OnMouseUp(MouseButtonEventArgs& e); 
		virtual void OnMouseWheelChanged(MouseWheelEventArgs& e) {}
		virtual void OnTextInput(TextInputEventArgs& e) {}
		virtual void OnTextEditing(TextEditingEventArgs& e) {}
	private:
		void UpdateStyleAndLayout();

		friend class Application;
		friend class SceneManager;
	public:
		static inline ObservableProperty<Scene, Ref<UIElement>> ContentProperty =
			ObservableProperty<Scene, Ref<UIElement>>(&SetContent, &GetContent);
	};
}