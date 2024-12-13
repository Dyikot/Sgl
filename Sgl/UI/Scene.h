#pragma once

#include <set>
#include <chrono>
#include "../Appearance/Color.h"
#include "../Appearance/Texture.h"
#include "../ECS/IProcessed.h"
#include "Controls/Control.h"

namespace Sgl
{	
	class Scene: public UIElement, public IProcessed
	{
	public:		
		static inline const PropertyId LoadedProperty = PropertyManager::Register<UIEventHandler>("Loaded");
		static inline const PropertyId UnloadedProperty = PropertyManager::Register<UIEventHandler>("Unloaded");
		
		Paint Background = &Colors::Black;
		std::set<Panel*, PanelComparer> Panels;
	private:
		Panel* _mouseOverPanel = nullptr;
		bool _isRunning = true;
	public:
		Scene() = default;
		Scene(const Style& style) noexcept;
		virtual ~Scene() = default;

		Event<UIEventHandler> Loaded;
		Event<UIEventHandler> Unloaded;

		void OnRender(RenderContext& renderContext) const override;
		void Close();
		bool IsClosed() const;
	protected:
		void OnMouseMove(const MouseButtonEventArgs& e) override;
		void OnMouseDown(const MouseButtonEventArgs& e) override;
		void OnMouseUp(const MouseButtonEventArgs& e) override;
		virtual void OnLoaded(const EventArgs& e);
		virtual void OnUnloaded(const EventArgs& e);
		virtual void OnTextChanged(const TextChangedEventArgs& e) {};
		virtual void OnTextInput(const TextInputEventArgs& e) {};
	private:
		friend class SceneManager;
		friend class Application;
	};
}