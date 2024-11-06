#pragma once

#include <list>
#include "Panels/Panel.h"
#include "../Render/Drawing.h"
#include "../Appearance/Color.h"
#include "../Appearance/Texture.h"

namespace Sgl
{	
	class SceneBackgroundDrawing: public Drawing
	{
	public:
		ColorBrush& Background;
	public:
		SceneBackgroundDrawing(ColorBrush& background):
			Background(background)
		{}

		void Draw() const override;
	private:
		void Fill(ColorBrush brush) const;
		void Fill(ImageBrush& brush) const;
	};

	class Scene: public UIElement
	{
	public:		
		static inline const PropertyId ClosedProperty = PropertyManager::Register<EventHandler>("Closed");
		static inline const PropertyId BackgroundProperty = PropertyManager::Register<ColorBrush>("Background");

		ColorBrush Background = Colors::Black;
	protected:
		SceneBackgroundDrawing _backgroundDrawing = Background;
	public:
		virtual ~Scene() = default;

		Event<EventHandler> Closed;

		virtual void Process() = 0;
		virtual void SetStyle(const Style& style) override;
		void OnRender() const override;
		void HandleEvent(const SDL_Event& e);
		void Close();
	protected:
		virtual void OnClose(const EventArgs& e);
	};
}