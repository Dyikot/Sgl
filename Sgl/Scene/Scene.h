#pragma once

#include "../Tools/Time/TimeSpan.h"
#include "../Visual/VisualElement.h"
#include "../Events/MouseAndKeyEvents.h"
#include "../UI/Layout/Layout.h"

namespace Sgl
{	
	class Scene: public VisualElement
	{
	public:
		shared_ptr<UI::Layout> Layout;
	public:
		Scene();
		Scene(const Scene&) = delete;
		Scene(Scene&&) = delete;
		virtual ~Scene() = default;

		void OnRender(RenderContext rc) const override;
		virtual void Process(TimeSpan elapsed);
		void ApplyStyle() override;
	protected:
		virtual void OnResumed() {}
		virtual void OnStopped() {}
		virtual void OnCreated();
		virtual void OnDestroying() {}

		friend class Application;
		friend class SceneManager;
	};
}