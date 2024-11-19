#pragma once

#include <list>
#include <chrono>
#include "../Appearance/Brushes.h"
#include "../Appearance/Texture.h"
#include "../ECS/IProcessed.h"
#include "UIElement.h"

namespace Sgl
{	
	class Scene: public UIElement, public IProcessed
	{
	public:		
		static inline const PropertyId ClosedProperty = PropertyManager::Register<EventHandler>("Closed");
		
		Brush Background = Colors::Black;
	public:
		Scene() = default;
		Scene(const Style& style) noexcept;
		virtual ~Scene() = default;

		Event<EventHandler> Closed;

		void OnRender(RenderContext& renderContext) const override;
		void HandleEvent(const SDL_Event& e);
		void Close();
	protected:
		virtual void OnClose(const EventArgs& e);
	};
}