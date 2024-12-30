#pragma once

#include <set>
#include <chrono>
#include "../Appearance/Color.h"
#include "../Appearance/Texture.h"
#include "../ECS/IProcessed.h"
#include "Panels/Panel.h"

namespace Sgl
{	
	class Scene: public UIElement, public IProcessed
	{
	public:		
		static inline const EventId LoadedProperty = EventManager::Register<UIEventHandler>("Loaded");
		static inline const EventId UnloadedProperty = EventManager::Register<UIEventHandler>("Unloaded");
		
		Fill Background = Colors::Black;
		ObjestSet<Panel> Panels;
	private:
		Panel* _mouseOverPanel = nullptr;
		bool _isActive = true;
	public:
		Scene() = default;
		virtual ~Scene() = default;

		void SetStyle(const Style& style) override;

		Event<UIEventHandler> Loaded;
		Event<UIEventHandler> Unloaded;

		void OnRender(RenderContext& renderContext) override;
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
		friend class SceneStack;
		friend class Application;
	};
}