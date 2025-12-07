#pragma once

#include <algorithm>
#include "Collection.h"
#include "../Event.h"

namespace Sgl
{
	enum class ObservableCollectionChangedAction
	{
		Add, Remove, Move, Replace, Reset
	};

	struct ObservableCollectionChangedEventArgs
	{
		ObservableCollectionChangedAction Action;
		size_t FromIndex;
		size_t ToIndex;
	};
	
	template<typename T>
	class ObservableCollection : public Collection<T>
	{
	public:
		using CollectionChangedEventHandler = EventHandler<ObservableCollection<T>, ObservableCollectionChangedEventArgs>;

		Event<CollectionChangedEventHandler> CollectionChanged;
	private:
		using base = Collection<T>;
	public:
		ObservableCollection() = default;
		ObservableCollection(const ObservableCollection&) = default;
		ObservableCollection(ObservableCollection&&) noexcept = default;

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
			ObservableCollectionChangedEventArgs args =
			{
				.Action = ObservableCollectionChangedAction::Reset,
				.FromIndex = 0,
				.ToIndex = 0
			};
			OnCollectionChanged(args);
		}

		void InsertItem(size_t index, const T& item) override
		{
			base::InsertItem(index, item);
			ObservableCollectionChangedEventArgs args =
			{
				.Action = ObservableCollectionChangedAction::Add,
				.FromIndex = index,
				.ToIndex = index
			};
			OnCollectionChanged(args);
		}

		void SetItem(size_t index, const T& item) override
		{
			base::SetItem(index, item);
			ObservableCollectionChangedEventArgs args
			{
				.Action = ObservableCollectionChangedAction::Replace,
				.FromIndex = index,
				.ToIndex = index
			};
			OnCollectionChanged(args);
		}

		void RemoveItem(size_t index) override
		{
			ObservableCollectionChangedEventArgs args
			{
				.Action = ObservableCollectionChangedAction::Remove,
				.FromIndex = index,
				.ToIndex = index
			};
			OnCollectionChanged(args);
			base::RemoveItem(index);
		}

		virtual void MoveItem(size_t fromIndex, size_t toIndex)
		{
			auto& items = base::_items;
			std::swap(items[fromIndex], items[toIndex]);
			ObservableCollectionChangedEventArgs args =
			{
				.Action = ObservableCollectionChangedAction::Move,
				.FromIndex = fromIndex,
				.ToIndex = toIndex
			};
			OnCollectionChanged(args);
		}

		virtual void OnCollectionChanged(ObservableCollectionChangedEventArgs& e)
		{
			CollectionChanged(*this, e);
		}
	};
}