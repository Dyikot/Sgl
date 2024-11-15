#pragma once

#include <list>
#include <chrono>
#include "Panels/Panel.h"
#include "../Appearance/Brushes.h"
#include "../Appearance/Texture.h"

namespace Sgl
{	
	class Scene: public UIElement
	{
	public:		
		static inline const PropertyId ClosedProperty = PropertyManager::Register<EventHandler>("Closed");
		
		Brush Background = Colors::Black;
	public:
		Scene() = default;
		Scene(const Style& style) noexcept;
		virtual ~Scene() = default;

		Event<EventHandler> Closed;

		virtual void Process(float elapsedMs) = 0;
		void OnRender(RenderContext& renderContext) const override;
		void HandleEvent(const SDL_Event& e);
		void Close();
	protected:
		virtual void OnClose(const EventArgs& e);
	};
}