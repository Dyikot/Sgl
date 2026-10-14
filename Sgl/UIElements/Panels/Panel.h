#pragma once

#include "../UIElement.h"

namespace Sgl
{
	class Panel;

	class UIElementsCollection
	{
	public:
		using const_iterator = std::vector<Ref<UIElement>>::const_iterator;
		using const_reverse_iterator = std::vector<Ref<UIElement>>::const_reverse_iterator;
	public:
		explicit UIElementsCollection(Panel& panel);
		UIElementsCollection(const UIElementsCollection&) = delete;
		UIElementsCollection(UIElementsCollection&&) = delete;

		const_iterator begin() const;
		const_iterator end() const;

		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend() const;

		size_t Count() const noexcept;
		bool IsEmpty() const noexcept;
		const Ref<UIElement>& Front() const;
		const Ref<UIElement>& Back() const;

		void Add(const Ref<UIElement>& item);
		void Insert(size_t index, const Ref<UIElement>& item);
		bool Remove(const Ref<UIElement>& item);
		void RemoveAt(size_t index);		
		void Clear();

		bool Contains(const Ref<UIElement>& item) const;
		Ref<UIElement> FindByName(std::string_view name) const;

		void SetElementAt(size_t index, const Ref<UIElement>& item);
		const Ref<UIElement>& GetElementAt(size_t index) const;
		const Ref<UIElement>& operator[](size_t index) const;

	private:
		Panel& _panel;
	};

	class Panel : public UIElement
	{
	public:
		Panel();

		UIElementsCollection Children;
	protected:
		FSize MeasureContent(FSize availableSize) override;
		void ArrangeContent(FRect rect) override;

		friend class UIElementsCollection;
	};
}