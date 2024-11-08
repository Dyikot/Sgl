#pragma once

#include <stack>
#include <filesystem>
#include <iostream>
#include "Collections/SceneCollection.h"
#include "Render/RenderContext.h"
#include "Appearance/Style/Style.h"
#include "Events/Event.h"

namespace Sgl
{
	class Window
	{
	public:
		friend class Application;

		enum class DiplayMode
		{
			Window, BorderlessWindow, Fullscreen
		};

		struct WindowProperties
		{
			std::string_view Title = "Window";
			SDL_Point Position = { .x = SDL_WINDOWPOS_CENTERED, .y = SDL_WINDOWPOS_CENTERED };
			size_t Width = 1280;
			size_t Height = 720;
			size_t LogicalWidth = Width;
			size_t LogicalHeight = Height;
			SDL_WindowFlags Flags = SDL_WindowFlags::SDL_WINDOW_SHOWN;
		};
		
		using WindowSizeChangedEventHandler = std::function<void(Window*, const SizeChangedEventArgs&)>;

		SDL_Window* const SDLWindow;
		SceneCollection Scenes;
	protected:
		RenderContext _renderContext;
		SDL_Surface* _icon = nullptr;
		int _width;
		int _height;		
	public:
		Window() noexcept;
		Window(const WindowProperties& properties) noexcept;
		~Window() noexcept;

		Event<WindowSizeChangedEventHandler> SizeChanged;
		
		void SetWidth(size_t width) noexcept;
		void SetHeight(size_t height) noexcept;
		void SetLogicalSize(size_t width, size_t height);
		void SetMaxSize(size_t width, size_t height);
		void SetTitle(std::string_view value);
		void SetPosition(SDL_Point value);
		void SetIcon(std::string_view path);
		void SetDisplayMode(DiplayMode displayMode);

		int	GetWidth() const noexcept { return _width; }
		int	GetHeight() const noexcept { return _height; }
		std::pair<size_t, size_t> GetLogicalSize() const;
		std::string_view GetTitle() const { SDL_GetWindowTitle(SDLWindow); }
		SDL_Point GetPosition() const;
		virtual RenderContext& GetRenderContext() { return _renderContext; }

		void Show();
		void Hide();
		void Close();
		bool IsVisible() const;
	protected:
		virtual void OnSizeChanged(const SizeChangedEventArgs& e);

		void Render();
	};
}