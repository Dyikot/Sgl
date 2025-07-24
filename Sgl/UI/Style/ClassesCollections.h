#pragma once

#include <vector>
#include <string>

namespace Sgl
{
	class ClassesCollection
	{
	private:
		std::vector<std::string> _items;
	public:
		ClassesCollection() = default;
		ClassesCollection(const ClassesCollection&) = default;
		ClassesCollection(ClassesCollection&&) noexcept = default;

		auto begin() { return _items.begin(); }
		auto end() { return _items.end(); }

		auto begin() const { return _items.begin(); }
		auto end() const { return _items.end(); }

		void Add(std::string className);
		void Set(const std::string& classes);
		void Remove(const std::string& className);
		size_t Count() const noexcept;
	private:
		static std::vector<std::string> Parse(const std::string& value);
	};
}