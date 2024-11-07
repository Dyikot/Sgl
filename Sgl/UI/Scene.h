#pragma once

#include <list>
#include "Panels/Panel.h"
#include "../Render/Drawing.h"
#include "../Appearance/Brushes.h"
#include "../Appearance/Texture.h"

namespace Sgl
{	
	class SceneBackgroundDrawing: public Drawing
	{
	public:
		Brush& Background;
	public:
		SceneBackgroundDrawing(Brush& brush):
			Background(brush)
		{}

		void Fill(const ColorBrush& brush) const;
		void Fill(const TextureBrush& brush) const;

		void Draw() const override;
	};

	class Scene: public UIElement
	{
	public:		
		static inline const PropertyId ClosedProperty = PropertyManager::Register<EventHandler>("Closed");
		
		Brush Background = Colors::Black;
	protected:
		SceneBackgroundDrawing _backgroundDrawing = Background;
	public:
		Scene() = default;
		Scene(const Style& style) noexcept;
		virtual ~Scene() = default;

		Event<EventHandler> Closed;

		virtual void Process() = 0;
		void OnRender() const override;
		void HandleEvent(const SDL_Event& e);
		void Close();
	protected:
		virtual void OnClose(const EventArgs& e);
	};
}