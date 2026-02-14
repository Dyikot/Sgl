#pragma once

#include <ranges>

#include "Window.h"
#include "Base/Media/ThemeMode.h"
#include "Base/Localization/StringLocalizer.h"
#include "Base/Localization/LocalizationStorage.h"

namespace Sgl
{
	/// <summary>
	/// Specifies when the application should shut down automatically.
	/// </summary>
	enum class ShutdownMode
	{
		OnMainWindowClose,    // The application exits when the main window is closed.
		OnLastWindowClose,    // The application exits when the last open window is closed.
		OnExplicitShutdown    // The application only shuts down when explicitly requested (e.g., via Shutdown()).
	};	

	/// <summary>
	/// Represents the main application instance, managing windows, styling, theme, and the event loop.
	/// </summary>
	class Application: public IStyleHost
	{
	public:
		/// <summary>
		/// Provides access to the current application instance via a global context.
		/// </summary>
		struct Context
		{
			Application* Current() { return Application::_current; }
			Application* operator->() { return Application::_current; }
		};
	private:
		using ApplicationEventHandler = EventHandler<Application>;
		using ThemeVariantChangedEventHanlder = EventHandler<Application>;
	public:
		/// <summary>
		/// Constructs an application instance.
		/// </summary>
		Application() noexcept;

		Application(const Application&) = delete;
		Application(Application&&) = delete;

		/// <summary>
		/// Destroys the application and releases all managed resources.
		/// </summary>
		~Application();

		/// <summary>
		/// Event raised when the application has started and entered its main loop.
		/// </summary>
		Event<ApplicationEventHandler> Started;

		/// <summary>
		/// Event raised when the application is shutting down and the main loop has exited.
		/// </summary>
		Event<ApplicationEventHandler> Stopped;

		/// <summary>
		/// Event raised when the active theme variant changes.
		/// </summary>
		Event<ThemeVariantChangedEventHanlder> ThemeVariantChanged;

		/// <summary>
		/// Collection of global styles applied to all UI elements in the application.
		/// </summary>
		StyleCollection Styles;

		/// <summary>
		/// The primary window of the application.
		/// </summary>
		Ref<Window> MainWindow;

		/// <summary>
		/// Determines when the application automatically shuts down.
		/// </summary>
		ShutdownMode ShutdownMode;

		/// <summary>
		/// Sets the active theme variant (e.g., Light, Dark, or System).
		/// If the new value differs from the current one, the ThemeVariantChanged event is triggered.
		/// When set to System, the application automatically follows the OS-level theme preference.
		/// </summary>
		/// <param name="value"> - the new theme variant to apply.</param>
		void SetThemeVariant(ThemeVariant value);

		/// <summary>
		/// Gets the currently active theme variant.
		/// </summary>
		/// <returns>The current theme variant.</returns>
		ThemeVariant GetThemeVariant() const { return _themeVariant; }

		/// <summary>
		/// Gets the current theme mode (Light or Dark).
		/// </summary>
		/// <returns>The detected theme mode.</returns>
		ThemeMode GetThemeMode() const noexcept { return _themeMode; }

		/// <summary>
		/// Gets the global style collection for this application.
		/// </summary>
		/// <returns>Reference to the application's StyleCollection.</returns>
		StyleCollection& GetStyles() final { return Styles; }

		/// <summary>
		/// Gets the parent styling host in the hierarchy.
		/// </summary>
		/// <returns>Always nullptr, as the application is the root style host.</returns>
		IStyleHost* GetStylingParent() final { return nullptr; }

		/// <summary>
		/// Gets the root styling host for the entire UI tree.
		/// </summary>
		/// <returns>This application instance, as it is the styling root.</returns>
		IStyleHost* GetStylingRoot() final { return this; }

		/// <summary>
		/// Gets a list of all currently open windows.
		/// </summary>
		/// <returns>A vector containing pointers to all active windows.</returns>
		const std::vector<Window*> GetWindows() const noexcept { return _activeWindows; }

		/// <summary>
		/// Creates LocalizationStorage from CSV file. Use StringLocalizer to get localized strings.
		/// </summary>
		/// <param name="csvFilePath"> - path to the CSV file containing localization data.</param>
		/// <param name="delimeter"> - character used as delimiter in the CSV file (default is comma).</param>
		void AddLocalization(std::string csvFilePath, char delimeter = ',');

		/// <summary>
		/// Starts the application's main event loop.
		/// Blocks until the application is shut down.
		/// </summary>
		void Run();

		/// <summary>
		/// Requests a shutdown of the application.
		/// The main loop will exit after processing pending events.
		/// </summary>
		void Shutdown();
	protected:
		virtual void OnStarted();
		virtual void OnStopped();
		virtual void OnThemeVariantChanged();
	private:
		ThemeMode GetSystemThemeMode() const;
		void HandleInputEvents();
		void PushSDLUserEvent(unsigned int type);
		void AddWindow(Window& window);
		void RemoveWindow(Window& window);
		void AttachWindow(Window& window);
		void DetachWindow(Window& window);
	private:
		friend class Window;
		friend class StringLocalizer;

		static inline Application* _current;

		bool _isRunning = false;
		ThemeMode _themeMode;
		ThemeVariant _themeVariant;
		std::unique_ptr<LocalizationStorage> _localizationStorage;		

		Window* _focusedWindow = nullptr;
		std::vector<Window*> _windows;
		std::vector<Window*> _activeWindows;
	};

	/// <summary>
	/// Global access point to the current application instance.
	/// Provides convenient syntax like App->GetThemeVariant() or App.Current()->MainWindow.
	/// </summary>
	inline Application::Context App;
}