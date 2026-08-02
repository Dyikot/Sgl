#pragma once

#include "IStyleHost.h"
#include "PseudoClass.h"
#include "../Data/Bindable.h"

namespace Sgl
{
	class Styleable : public Bindable, public IStyleHost
	{
	public:
		using StyleableElementEventHandler = EventHandler<Styleable>;
	public:
		Styleable();
		Styleable(Styleable&& other) noexcept;

		std::string Name;
		StyleCollection Styles;
		PseudoClassesSet PseudoClasses;
		Event<StyleableElementEventHandler> AttachedToLogicalTree;
		Event<StyleableElementEventHandler> DetachedFromLogicalTree;

		void SetClasses(std::string_view classNames);
		void SetClasses(std::vector<std::string> classList);
		const std::vector<std::string>& GetClasses() const;

		StyleCollection& GetStyles() final { return Styles; }
		IStyleHost* GetStylingParent() final { return _stylingParent; }
		bool IsAttachedToLogicalTree() const noexcept { return _isAttachedToLogicalTree; }

		void ApplyStyle();
	protected:
		virtual void SetParent(IStyleHost* parent);
		virtual void OnAttachedToLogicalTree();
		virtual void OnDetachedFromLogicalTree();
		void OnDataContextChanged(const Ref<INotifyPropertyChanged>& dataContext) final;
		void AddLogicalChild(Styleable* child);
		void RemoveLogicalChild(Styleable* child);
		const std::vector<Styleable*>& GetLogicalChildren() const { return _logicalChildren; }
	private:
		bool FetchStyles();
		void FetchStylesFrom(const StyleCollection& styles);
		void OnStyleClassesChanged();
		void ApplyStateStyle();
		void SaveBaseState();
		void RestoreBaseState();
		void ClearMatchingStateStyles();
		bool MatchStateStyles();
	private:
		class SavedState
		{
		public:
			SavedState(Action<> restore, 
					   Styleable* target, 
					   StyleableElementEventHandler detachedHandler);
			SavedState(const SavedState&) = delete;
			SavedState(SavedState&& other) noexcept;
			~SavedState();

			Styleable* GetTarget() const noexcept;

			SavedState& operator=(const SavedState&) = delete;
			SavedState& operator=(SavedState&& other) noexcept;
		private:
			Action<> _restore;
			Styleable* _target;
			StyleableElementEventHandler _detachedHandler;
		};

		std::vector<Styleable*> _logicalChildren;
		std::vector<std::string> _classList;
		std::vector<const Style*> _styles;
		std::vector<const Style*> _stateStyles;
		std::vector<const Style*> _matchingStateStyles;
		std::vector<SavedState> _savedStates;
		IStyleHost* _stylingParent = nullptr;
		bool _isAttachedToLogicalTree = false;

		friend class EraseSavedStateHandler;
	};
}