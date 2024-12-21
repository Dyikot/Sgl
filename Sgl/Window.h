#pragma once

#include <stack>
#include <filesystem>
#include <iostream>
#include "Collections/SceneManager.h"
#include "UI/Scene.h"
#include "Render/RenderContext.h"
#include "Appearance/Style/Style.h"
#include "Events/EventArgs.h"
#include "Appearance/Surface.h"

namespace Sgl
{
	enum class DiplayMode
	{
		Window, BorderlessWindow, Fullscreen
	};

	enum class WindowState
	{
		Normal, Minimized, Maximized
	};

	class Window
	{
	public:		
		using WindowEventHandler = EventHandler<Window, EventArgs>;
		using WindowSizeChangedEventHandler = EventHandler<Window, SizeChangedEventArgs>;

		SceneManager Scenes;
	protected:
		SDL_Window* const _sdlWindow;
		RenderContext _renderContext;
		std::optional<Surface> _icon = std::nullopt;
		int _width;
		int _height;
		bool _isVsyncEnable = false;
	public:
		Window() noexcept;
		Window(std::string_view title, SDL_Point position, 
			   size_t width, size_t height,
			   size_t logicalWidth, size_t logicalHeight,
			   SDL_WindowFlags flags) noexcept;
		Window(const Window&) = delete;
		Window(Window&&) = delete;
		~Window() noexcept { SDL_DestroyWindow(_sdlWindow); }

		Event<WindowEventHandler> StateChanged;
		Event<WindowSizeChangedEventHandler> SizeChanged;
		
		void SetWidth(size_t width) noexcept;
		void SetHeight(size_t height) noexcept;
		void SetLogicalSize(size_t width, size_t height);
		void SetMaxSize(size_t width, size_t height);
		void SetTitle(std::string_view value);
		void SetPosition(SDL_Point value);
		void SetIcon(Surface&& icon);
		void SetIcon(const Surface& icon);
		void SetDisplayMode(DiplayMode displayMode);

		int	GetWidth() const noexcept { return _width; }
		int	GetHeight() const noexcept { return _height; }
		std::pair<size_t, size_t> GetLogicalSize() const;
		std::string_view GetTitle() const { SDL_GetWindowTitle(_sdlWindow); }
		SDL_Point GetPosition() const;
		WindowState GetWindowState() const;
		virtual RenderContext& GetRenderContext() { return _renderContext; }

		void Render();
		void Show();
		void Hide();
		void Close();
		bool IsVisible() const;
		void EnableVsync();
		void DisableVsync();
		bool IsVsyncEnable() const { _isVsyncEnable; }

		operator SDL_Window* () const { return _sdlWindow; }
	protected:
		virtual void OnSizeChanged(const SizeChangedEventArgs& e);
		virtual void OnStateChanged(const EventArgs& e);
	private:
		friend class Application;
	};

	class WindowBuilder
	{
	private:
		std::string _title = "Window";
		SDL_Point _position = SDL_Point{ SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED };
		size_t _width = 1280;
		size_t _height = 720;
		size_t _logicalWidth = 1280;
		size_t _logicalHeight = 720;
		SDL_WindowFlags _flags = SDL_WINDOW_SHOWN;
	public:
		WindowBuilder& Title(std::string&& value) { _title = std::move(value); return *this; }
		WindowBuilder& Position(SDL_Point value) { _position = value; return *this; }
		WindowBuilder& Width(size_t value) { _width = value; _logicalWidth = value;  return *this; }
		WindowBuilder& Height(size_t value) { _height = value; _logicalHeight = value; return *this; }
		WindowBuilder& LogicalWidth(size_t value) { _logicalWidth = value; return *this; }
		WindowBuilder& LogicalHeight(size_t value) { _logicalHeight = value; return *this; }
		WindowBuilder& Flags(SDL_WindowFlags value) { _flags = value; return *this; }
		Window Build()
		{
			return Window(_title, _position, _width, _height, _logicalWidth, _logicalHeight, _flags);
		}
	};
}