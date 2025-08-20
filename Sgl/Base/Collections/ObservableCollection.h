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
			OnCollectionChanged(NotifyCollectionChangedEventArgs
			{
				.Action = NotifyCollectionChangedAction::Reset,
				.StartIndex = 0,
				.EndIndex = 0
			});
		}

		void InsertItem(size_t index, const T& item) override
		{
			base::InsertItem(index, item);
			OnCollectionChanged(NotifyCollectionChangedEventArgs
			{
				.Action = NotifyCollectionChangedAction::Add,
				.StartIndex = index,
				.EndIndex = index
			});
		}

		void SetItem(size_t index, const T& item) override
		{
			base::SetItem(index, item);
			OnCollectionChanged(NotifyCollectionChangedEventArgs
			{
				.Action = NotifyCollectionChangedAction::Replace,
				.StartIndex = index,
				.EndIndex = index
			});
		}

		void RemoveItem(size_t index) override
		{
			base::RemoveItem(index);
			OnCollectionChanged(NotifyCollectionChangedEventArgs
			{
				.Action = NotifyCollectionChangedAction::Remove,
				.StartIndex = index,
				.EndIndex = index
			});
		}

		virtual void MoveItem(size_t fromIndex, size_t toIndex)
		{
			auto& items = base::Items();
			std::swap(items[fromIndex], items[toIndex]);
			OnCollectionChanged(NotifyCollectionChangedEventArgs
			{
				.Action = NotifyCollectionChangedAction::Move,
				.StartIndex = fromIndex,
				.EndIndex = toIndex
			});
		}

		virtual void OnCollectionChanged(const NotifyCollectionChangedEventArgs& e)
		{
			CollectionChanged.TryInvoke(*this, e);
		}
	};
}