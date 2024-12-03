#pragma once

#include <list>
#include <chrono>
#include "../Appearance/Brushes.h"
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
		
		Brush Background = Colors::Black;
		std::vector<Control*> Controls;
	private:
		bool _isClosed = false;
	public:
		Scene() = default;
		Scene(const Style& style) noexcept;
		virtual ~Scene() = default;

		Event<UIEventHandler> Loaded;
		Event<UIEventHandler> Unloaded;

		void OnRender(RenderContext& renderContext) const override;
		void Close();
		bool IsClosed() const noexcept;
	protected:
		Control* FindControl(SDL_Point mousePosition);
		virtual void OnLoaded(const EventArgs& e);
		virtual void OnUnloaded(const EventArgs& e);
		virtual void OnTextChanged(const TextChangedEventArgs& e) {};
		virtual void OnTextInput(const TextInputEventArgs& e) {};
	private:
		friend class SceneStack;
		friend class Application;
	};
}