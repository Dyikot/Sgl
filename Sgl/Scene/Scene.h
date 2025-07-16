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
		Cursor Cursor;
		Color BackgroundColor;
		shared_ptr<Texture> BackgroundTexture;

		ResourcesMap Resources;
		StyleMap Styles;
	protected:
		shared_ptr<UIElement> _uielement;
		IStyleProvider* _stylingParent;
	public:
		Scene();
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		virtual ~Scene() = default;

		void SetUIElement(shared_ptr<UIElement> value);
		shared_ptr<UIElement> GetUIElement() const { return _uielement; }

		StyleMap& GetStyles() override { return Styles; }
		IStyleProvider* GetStylingParent() { return _stylingParent; }

		void OnRender(RenderContext context) const override;
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
		void UpdateInvalidatedLayout();

		friend class Application;
		friend class SceneManager;
	};
}