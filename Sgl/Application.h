#pragma once

#include <SDL/SDL_ttf.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include "Window.h"
#include "Base/Localization/StringLocalizerBase.h"
#include <ranges>

namespace Sgl
{
	class Application
	{
	private:
		using ApplicationEventHandler = EventHandler<Application>;
	public:
		struct Context
		{
			Application* Current() { return Application::_current; }
			Application* operator->() { return Application::_current; }
			const Application* operator->() const { return Application::_current; }
		};

		Event<ApplicationEventHandler> Started;
		Event<ApplicationEventHandler> Stopped;

		StyleMap Styles;
	private:
		static inline Application* _current;

		int _visibleWindows = 0;
		bool _isRunning = false;
		std::string _culture = "en";
		std::vector<Window*> _windows;
		Window* _currentWindow = nullptr;
		mutable std::unique_ptr<Window> _mainWindow;
		std::unique_ptr<StringLocalizerBase> _localizer;
	public:
		Application() noexcept;
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		~Application();

		void SetCulture(const std::string& value);
		const std::string& GetCulture() const { return _culture; }

		void SetLocalizer(std::string csvFile, char delimeter = ',');
		void SetLocalizer(std::unique_ptr<StringLocalizerBase> localizer);
		const StringLocalizerBase& GetLocalizer() const;

		void SetMainWindow(std::unique_ptr<Window> value);
		Window& GetMainWindow() const;

		const std::vector<Window*> GetWindows() const noexcept;

		void Run();
		void Shutdown();
	protected:
		virtual void OnRun();
		virtual void OnStop();
	private:
		void HandleEvents();
		void AddWindow(Window* window);
		void RemoveWindow(Window* window);
		Window* GetWindowById(int id);

		friend class Window;
	};	

	inline Application::Context App;
} 