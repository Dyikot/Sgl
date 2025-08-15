#pragma once

#include "../UI/UIElement.h"
#include "../Render/IRenderable.h"
#include "../Base/Time/TimeSpan.h"
#include "../Input/TextEventArgs.h"
#include "../Base/Media/Brush.h"
#include "../Base/Ref.h"

namespace Sgl
{	
	class Scene: public IRenderable, public IStyleProvider
	{
	public:
		ResourcesMap Resources;
		StyleMap Styles;
	protected:
		Ref<UIElement> _content;
		IStyleProvider* _stylingParent;
	private:
		Brush _background = Colors::Black;
		Cursor _cursor = Cursors::Arrow;
		bool _isRenderValid = false;
	public:
		Scene();
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		virtual ~Scene() = default;

		void SetBackground(Brush value) { _background = std::move(value); }
		Brush GetBackground() const { return _background; }

		void SetCursor(const Cursor& value);
		const Cursor& GetCursor() const { return _cursor; }

		void SetContent(Ref<UIElement> value);
		Ref<UIElement> GetContent() { return _content; }

		StyleMap& GetStyles() override { return Styles; }
		IStyleProvider* GetStylingParent() { return _stylingParent; }
		bool NeedsRendering() const noexcept { return !_isRenderValid; }

		void Render(RenderContext context) override;
		void InvalidateRender() { _isRenderValid = false; }
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
		void UpdateLayoutAndStyle();

		friend class Application;
		friend class SceneManager;
	};
}