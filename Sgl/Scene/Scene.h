#pragma once

#include "../UI/UIElement.h"
#include "../Render/IRenderable.h"
#include "../Base/Time/TimeSpan.h"
#include "../Input/MouseAndKeyArgs.h"

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
	private:
		void UpdateInvalidatedLayout();

		friend class Application;
		friend class SceneManager;
	};
}