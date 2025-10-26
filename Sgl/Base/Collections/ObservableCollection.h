#pragma once

#include <algorithm>
#include "Collection.h"
#include "../Event.h"

namespace Sgl
{
	enum class NotifyCollectionChangedAction
	{
		Add, Remove, Move, Replace, Reset
	};

	struct NotifyCollectionChangedEventArgs : EventArgs
	{
		NotifyCollectionChangedAction Action;
		size_t StartIndex;
		size_t EndIndex;
	};
	
	template<typename T>
	class ObservableCollection : public Collection<T>
	{
	public:
		using NofityCollectionChangedEventHandler = EventHandler<ObservableCollection<T>, NotifyCollectionChangedEventArgs>;

		Event<NofityCollectionChangedEventHandler> CollectionChanged;
	private:
		using base = Collection<T>;
	public:
		ObservableCollection() = default;
		ObservableCollection(const ObservableCollection&) = default;
		ObservableCollection(ObservableCollection&&) noexcept = default;
		virtual ~ObservableCollection() = default;

		ObservableCollection(std::initializer_list<T> init):
			base(init)
		{}

		ObservableCollection(std::vector<T> items):
			base(std::move(items))
		{}

		void Move(size_t fromIndex, size_t toIndex)
		{
			MoveItem(fromIndex, toIndex);
		}
	protected:
		void ClearItems() override
		{
			base::ClearItems();
			NotifyCollectionChangedEventArgs args =
			{
				.Action = NotifyCollectionChangedAction::Reset,
				.StartIndex = 0,
				.EndIndex = 0
			};
			OnCollectionChanged(args);
		}

		void InsertItem(size_t index, const T& item) override
		{
			base::InsertItem(index, item);
			NotifyCollectionChangedEventArgs args =
			{
				.Action = NotifyCollectionChangedAction::Add,
				.StartIndex = index,
				.EndIndex = index
			};
			OnCollectionChanged(args);
		}

		void SetItem(size_t index, const T& item) override
		{
			base::SetItem(index, item);
			NotifyCollectionChangedEventArgs args
			{
				.Action = NotifyCollectionChangedAction::Replace,
				.StartIndex = index,
				.EndIndex = index
			};
			OnCollectionChanged(args);
		}

		void RemoveItem(size_t index) override
		{
			base::RemoveItem(index);
			NotifyCollectionChangedEventArgs args
			{
				.Action = NotifyCollectionChangedAction::Remove,
				.StartIndex = index,
				.EndIndex = index
			};
			OnCollectionChanged(args);
		}

		virtual void MoveItem(size_t fromIndex, size_t toIndex)
		{
			auto& items = base::Items();
			std::swap(items[fromIndex], items[toIndex]);
			NotifyCollectionChangedEventArgs args =
			{
				.Action = NotifyCollectionChangedAction::Move,
				.StartIndex = fromIndex,
				.EndIndex = toIndex
			};
			OnCollectionChanged(args);
		}

		virtual void OnCollectionChanged(NotifyCollectionChangedEventArgs& e)
		{
			CollectionChanged.TryInvoke(*this, e);
		}
	};
}