#pragma once

#include <stack>
#include <queue>
#include "../ECS/IProcessed.h"
#include "UIElement.h"

namespace Sgl
{	
	class Scene: 
		public IVisual, 
		public ECS::IProcessed, 
		public IKeyEventsListener
	{
	public:
		struct Properties
		{
			Cursor::Getter Cursor = Cursors::Arrow;
			Color BackgroundColor = Colors::Black;
			Texture* BackgroundTexture = nullptr;
		};

		using PropertiesFactory = std::unique_ptr<Properties>(*)();

		Style<Properties> BaseStyle;
		UIElementsCollection UIElements;
		Event<KeyEventHandler> KeyUp;
		Event<KeyEventHandler> KeyDown;
	protected:
		std::unique_ptr<Properties> _properties;
	public:
		Scene(PropertiesFactory propertiesFactory);
		Scene();
		virtual ~Scene() = default;

		void OnRender(RenderContext renderContext) const override;

		void SetCursor(Cursor::Getter value) { _properties->Cursor = value; }
		Cursor::Getter GetCursor() const { return _properties->Cursor; }

		void SetBackgroundColor(Color value) { _properties->BackgroundColor = value; }
		Color GetBackgroundColor() const { return _properties->BackgroundColor; }

		void SetBackgroundTexture(Texture* value) { _properties->BackgroundTexture = value; }
		const Texture* GetBackgroundTexture() const { _properties->BackgroundTexture; }
	protected:
		virtual void OnKeyDown(const KeyEventArgs& e) override
		{
			KeyDown.TryRaise(*this, e);
		}

		virtual void OnKeyUp(const KeyEventArgs& e) override
		{
			KeyUp.TryRaise(*this, e);
		}
	private:
		friend class Application;
	};

	class SceneManager
	{
	private:
		using SceneFactory = Func<std::shared_ptr<Scene>>;

		std::stack<std::shared_ptr<Scene>> _scenes;
		std::queue<SceneFactory> _sceneFactoriesQueue;
		size_t _popScenes = 0;
	public:
		template<std::derived_from<Scene> TScene>
		void Push() noexcept
		{
			_sceneFactoriesQueue.push(&std::make_shared<TScene>);
		}

		void Push(SceneFactory sceneFactory);

		void Pop() noexcept;
		std::shared_ptr<Scene> GetCurrentScene();
	private:
		std::shared_ptr<Scene> GetNextScene();
		void CreateScene() noexcept;
		void DestroyScene() noexcept;

		friend class Application;
	};
}