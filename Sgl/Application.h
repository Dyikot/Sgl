#pragma once

#include <ranges>
#include "Window.h"
#include "Base/Localization/StringLocalizerBase.h"

namespace Sgl
{
	enum class ThemeMode
	{
		System, Light, Dark
	};

	enum class SystemTheme
	{
		Light = 1, Dark
	};

	struct ThemeModeChangedEventArgs
	{
		ThemeMode Theme;
	};

	struct SystemModeChangedEventArgs
	{
		SystemTheme Theme;
	};

	class Application
	{
	private:
		using ApplicationEventHandler = EventHandler<Application>;
		using ThemeModeChangedEventHanlder = EventHandler<Application, ThemeModeChangedEventArgs>;
		using SystemThemeChangedEventHanlder = EventHandler<Application, SystemModeChangedEventArgs>;
	public:
		struct Context
		{
			Application* Current() { return Application::_current; }
			Application* operator->() { return Application::_current; }
			const Application* operator->() const { return Application::_current; }
		};

		Event<ApplicationEventHandler> Started;
		Event<ApplicationEventHandler> Stopped;
		Event<ThemeModeChangedEventHanlder> ThemeModeChanged;
		Event<SystemThemeChangedEventHanlder> SystemThemeChanged;

		StyleMap Styles;
	private:
		static inline Application* _current;

		bool _isRunning = false;
		TimeSpan _delayDuration = TimeSpan::FromMicroseconds(16666);
		Stopwatch _stopwatch;
		ThemeMode _themeMode;
		SystemTheme _systemTheme;
		std::string _culture = "en";
		Window* _focusedWindow = nullptr;
		std::vector<Window*> _activeWindows;
		std::unordered_map<int, Window*> _windows;
		std::unique_ptr<Window> _mainWindow;
		std::unique_ptr<StringLocalizerBase> _localizer;
	public:
		Application() noexcept;
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		virtual ~Application();

		void SetThemeMode(ThemeMode value);
		ThemeMode GetThemeMode() const;
		SystemTheme GetSystemTheme() const;

		void SetCulture(const std::string& value);
		const std::string& GetCulture() const { return _culture; }

		void SetLocalizer(std::string csvFile, char delimeter = ',');
		void SetLocalizer(std::unique_ptr<StringLocalizerBase> localizer);
		const StringLocalizerBase& GetLocalizer() const;

		void SetMainWindow(std::unique_ptr<Window> value);
		Window* GetMainWindow() const;

		const std::vector<Window*> GetWindows() const noexcept;

		void Run();
		void Shutdown();
	protected:
		virtual void OnRun();
		virtual void OnStop();
		virtual void OnThemeModeChanged(ThemeModeChangedEventArgs theme);
		virtual void OnSystemThemeChanged(SystemModeChangedEventArgs theme);
	private:
		SystemTheme QuerySystemTheme() const;
		void Delay();
		void HandleEvents();
		void AddWindow(Window* window);
		void AddActiveWindow(Window* window);
		void RemoveWindow(Window* window);
		void RemoveActiveWindow(Window* window);
		Window* GetWindowById(int id);

		friend class Window;
	};	

	inline Application::Context App;
} 