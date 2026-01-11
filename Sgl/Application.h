#pragma once

#include <ranges>

#include "Window.h"
#include "Base/Media/ThemeVartiant.h"
#include "Base/Localization/StringLocalizerBase.h"

namespace Sgl
{
	enum class ShutdownMode
	{
		OnMainWindowClose, OnLastWindowClose, OnExplicitShutdown
	};

	class Application : public IStyleHost
	{
	private:
		using ApplicationEventHandler = EventHandler<Application>;
		using ThemeVariantChangedEventHanlder = EventHandler<Application, ThemeVariantChangedEventArgs>;
	public:
		struct Context
		{
			Application* Current() { return Application::_current; }
			Application* operator->() { return Application::_current; }
		};

		Event<ApplicationEventHandler> Started;
		Event<ApplicationEventHandler> Stopped;
		Event<ThemeVariantChangedEventHanlder> ThemeVariantChanged;

		StyleCollection Styles;
		Ref<Window> MainWindow;
		ShutdownMode ShutdownMode;
	private:
		static inline Application* _current;

		bool _isRunning = false;
		ThemeMode _themeMode;
		ThemeVariant _themeVariant;
		std::string _culture = "en";
		Ref<StringLocalizerBase> _localizer;

		Window* _focusedWindow = nullptr;
		std::vector<Window*> _windows;
		std::vector<Window*> _activeWindows;
	public:
		Application() noexcept;
		Application(const Application&) = delete;
		Application(Application&&) = delete;
		~Application();

		void SetThemeVariant(ThemeVariant value);
		ThemeVariant GetThemeVariant() const { return _themeVariant; }

		ThemeMode GetThemeMode() const noexcept { return _themeMode; }

		void SetCulture(const std::string& value);
		const std::string& GetCulture() const { return _culture; }

		void SetLocalizer(const Ref<StringLocalizerBase>& localizer);
		const Ref<StringLocalizerBase>& GetLocalizer() const { return _localizer; }

		StyleCollection& GetStyles() final { return Styles; }
		IStyleHost* GetStylingParent() final { return nullptr; }
		IStyleHost* GetStylingRoot() final { return this; }

		const std::vector<Window*> GetWindows() const noexcept { return _activeWindows; }

		void Run();
		void Shutdown();
	protected:
		virtual void OnStarted();
		virtual void OnStopped();
		virtual void OnThemeVariantChanged(ThemeVariantChangedEventArgs e);
	private:
		ThemeMode GetSystemThemeMode() const;
		void HandleInputEvents();
		void PushSDLUserEvent(unsigned int type);
		void AddWindow(Window& window);
		void RemoveWindow(Window& window);
		void AttachWindow(Window& window);
		void DetachWindow(Window& window);

		friend class Window;
	};	

	inline Application::Context App;
} 