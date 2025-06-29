#pragma once

#include "../Tools/Time/TimeSpan.h"
#include "../Render/IRenderable.h"
#include "../Events/MouseAndKeyArgs.h"
#include "../UI/Layout/Layout.h"

namespace Sgl
{	
	class Scene: public IRenderable
	{
	public:
		Color BackgroundColor;
		shared_ptr<Texture> BackgroundTexture;
		shared_ptr<Layout> Layout;
	public:
		Scene();
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		virtual ~Scene() = default;

		void OnRender(RenderContext rc) const override;
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