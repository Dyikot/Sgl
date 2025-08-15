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
		Ref<UIElement> GetContent() { return _content; }

		void Render(RenderContext context) override;
		virtual void Process(TimeSpan elapsed);
	protected:
		void OnCursorChanged(const Cursor& cursor) override;
		virtual void OnResumed() {}
		virtual void OnStopped() {}
		virtual void OnCreated();
		virtual void OnDestroying() {}
		virtual void OnKeyUp(const KeyEventArgs& e) {}
		virtual void OnKeyDown(const KeyEventArgs& e) {}
		virtual void OnMouseMove(const MouseEventArgs& e); 
		virtual void OnMouseDown(const MouseButtonEventArgs& e); 
		virtual void OnMouseUp(const MouseButtonEventArgs& e); 
		virtual void OnMouseWheelChanged(const MouseWheelEventArgs& e) {}
		virtual void OnTextInput(const TextInputEventArgs& e) {}
		virtual void OnTextEditing(const TextEditingEventArgs& e) {}
	private:
		void UpdateStyleAndLayout();

		friend class Application;
		friend class SceneManager;
	};
}