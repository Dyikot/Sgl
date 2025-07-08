#pragma once

#include "../UI/Layout.h"
#include "../Render/IRenderable.h"
#include "../Base/Time/TimeSpan.h"
#include "../Base/Input/MouseAndKeyArgs.h"

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
		shared_ptr<Layout> _layout;
		IStyleProvider* _stylingParent;
	public:
		Scene();
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		virtual ~Scene() = default;

		void SetLayout(shared_ptr<Layout> value);
		shared_ptr<Layout> GetLayout() const { return _layout; }

		StyleMap& GetStyles() override { return Styles; }
		IStyleProvider* GetStylingParent() { return _stylingParent; }

		void OnRender(RenderContext context) const override;
		virtual void Process(TimeSpan elapsed);
	protected:
		virtual void OnResumed() {}
		virtual void OnStopped() {}
		virtual void OnCreated();
		virtual void OnDestroying() {}

		friend class Application;
		friend class SceneManager;
	};
}