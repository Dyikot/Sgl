#pragma once

#include <ranges>
#include <algorithm>
#include <functional>
#include <optional>

namespace Sgl::Ranges
{
	template<std::ranges::range TRange>
	using TRangeValue = std::ranges::range_value_t<TRange>;

	template<typename TAdaptor>
	struct RangeAdaptor
	{
		constexpr auto operator()(std::ranges::range auto&& range) const
		{
			return static_cast<const TAdaptor&>(*this)(std::forward<decltype(range)>(range));
		}

		template<std::ranges::range TRange>
		constexpr friend decltype(auto) operator|(TRange&& range, RangeAdaptor&& adaptor)
		{
			return adaptor(std::forward<TRange>(range));
		}
	};

	template<typename TFunc>
	struct _AggregateAdaptor: public RangeAdaptor<_AggregateAdaptor<TFunc>>
	{
		TFunc function;

		constexpr _AggregateAdaptor(TFunc&& func):
			function(std::move(func)) {}

		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			TRangeValue<TRange> result = {};
			for(const auto& item : range)
			{
				result = function(result, item);
			}

			return result;
		}
	};

	template<typename TFunc, typename TAccumulate>
	struct _AgregateAdaptor2: public RangeAdaptor<_AgregateAdaptor2<TFunc, TAccumulate>>
	{
		TFunc function;
		TAccumulate seed;

		constexpr _AgregateAdaptor2(TFunc&& func, const TAccumulate& seed):
			function(std::move(func)),
			seed(seed)
		{}

		template<std::ranges::range TRange>
		constexpr TAccumulate operator()(TRange&& range) const
		{
			TAccumulate result = seed;
			for(const auto& item : range)
			{
				result = function(result, item);
			}

			return result;
		}
	};

	template<typename TFunc>
	constexpr auto Aggregate(TFunc func)
	{
		return _AggregateAdaptor<TFunc>(func);
	}

	template<typename TFunc, typename TAccumulate>
	constexpr auto Aggregate(TFunc func, const TAccumulate& seed)
	{
		return _AgregateAdaptor2<TFunc, TAccumulate>(std::move(func), seed);
	}

	template<typename TPredicate>
	struct _AllAdaptor: public RangeAdaptor<_AllAdaptor<TPredicate>>
	{
		TPredicate predicate;

		constexpr _AllAdaptor(TPredicate&& predicate):
			predicate(std::move(predicate)) {}

		template<std::ranges::range TRange>
		constexpr bool operator()(TRange&& range) const
		{
			for(const auto& item : range)
			{
				if(!predicate(item))
				{
					return false;
				}
			}

			return true;
		}
	};

	template<typename TPredicate>
	constexpr auto All(TPredicate predicate)
	{
		return _AllAdaptor<TPredicate>(std::move(predicate));
	}

	template<typename TPredicate>
	struct _AnyAdaptor: public RangeAdaptor<_AnyAdaptor<TPredicate>>
	{
		TPredicate predicate;

		constexpr _AnyAdaptor(TPredicate&& predicate):
			predicate(std::move(predicate)) {}

		template<std::ranges::range TRange>
		constexpr bool operator()(TRange&& range) const
		{
			for(const auto& item : range)
			{
				if(predicate(item))
				{
					return true;
				}
			}

			return false;
		}
	};

	template<typename TPredicate>
	constexpr auto Any(TPredicate predicate)
	{
		return _AnyAdaptor<TPredicate>(std::move(predicate));
	}

	struct _AverageAdaptor: public RangeAdaptor<_AverageAdaptor>
	{		
		template<std::ranges::range TRange>
		constexpr double operator()(TRange&& range) const
		{
			double sum = 0;

			for(const auto& item : range)
			{
				sum += item;
			}
			
			return sum / std::ranges::size(range);
		}
	};

	constexpr auto Average()
	{
		return _AverageAdaptor();
	}

	template<typename TResult>
	constexpr auto Cast()
	{
		return std::views::transform([](const auto& item) 
		{
			return static_cast<TResult>(item);
		});
	}

	struct _ChunkAdaptor: public RangeAdaptor<_ChunkAdaptor>
	{
		size_t size;

		constexpr _ChunkAdaptor(size_t size):
			size(size)
		{}

		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			if(size == 0)
			{
				throw std::invalid_argument("Chunk size must be more than 0");
			}

			const auto RangeSize = std::ranges::size(range);
			const auto ChunksNumber = RangeSize / size;
			std::vector<std::vector<TRangeValue<TRange>>> chunks(ChunksNumber);
			auto it = range.begin();

			for(auto& chunk : chunks)
			{
				auto c = std::views::counted(it, size);
				chunk = std::vector(c.begin(), c.end());
				it += size;
			}
						
			if(it != range.end())
			{
				auto left = std::ranges::distance(it, range.end());
				auto c = std::views::counted(it, left);
				chunks.push_back(std::vector(c.begin(), c.end()));
			}

			return chunks;
		}
	};

	constexpr auto Chunk(size_t size)
	{
		return _ChunkAdaptor(size);
	}

	struct _CountAdaptor: public RangeAdaptor<_CountAdaptor>
	{
		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			return std::ranges::size(range);
		}
	};

	constexpr auto Count()
	{
		return _CountAdaptor();
	}

	/*template<std::ranges::view TView>
	struct _ConcatAdaptor: public RangeAdaptor<_ConcatAdaptor<TView>>
	{
		TView View;

		constexpr _ConcatAdaptor(TView view):
			View(view)
		{}

		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			
		}
	};

	constexpr auto Concat(std::ranges::viewable_range auto&& range)
	{
		auto view = std::views::all(std::forward<decltype(range)>(range));
		return _ConcatAdaptor<decltype(view)>(view);
	}*/

	template<typename T>
	struct _ContainsAdaptor: public RangeAdaptor<_ContainsAdaptor<T>>
	{
		T value;

		constexpr _ContainsAdaptor(const T& value):
			value(value)
		{}

		template<std::ranges::range TRange>
		constexpr bool operator()(TRange&& range) const
			requires std::convertible_to<TRangeValue<TRange>, T>
		{
			for(const auto& item : range)
			{
				if(item == value)
				{
					return true;
				}
			}

			return false;
		}
	};

	template<typename T>
	constexpr auto Contains(const T& value)
	{
		return _ContainsAdaptor<T>(value);
	}

	/*struct _DistinctAdaptor: public RangeAdaptor<_DistinctAdaptor>
	{
		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			
		}
	};

	constexpr auto Distinct()
	{
		return _DistinctAdaptor();
	}*/

	/*template<typename TKeySelector>
	struct _DistinctByAdaptor: public RangeAdaptor<_DistinctByAdaptor>
	{
		TKeySelector KeySelector;

		constexpr _DistinctByAdaptor(TKeySelector keySelector):
			KeySelector(std::move(keySelector))
		{}

		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{

		}
	};

	template<typename TKeySelector>
	constexpr auto DistinctBy(TKeySelector keySelector)
	{
		return _DistinctByAdaptor<TKeySelector>(std::move(keySelector));
	}*/

	struct _ElementAtAdaptor: public RangeAdaptor<_ElementAtAdaptor>
	{
		size_t position;

		constexpr _ElementAtAdaptor(size_t position):
			position(position)
		{}

		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			auto it = range.begin();
			std::ranges::advance(it, position);
			return *it;
		}
	};

	constexpr auto ElementAt(size_t position)
	{
		return _ElementAtAdaptor(position);
	}

	struct _ElementAtOrDefaultAdaptor: public RangeAdaptor<_ElementAtOrDefaultAdaptor>
	{
		size_t position;

		constexpr _ElementAtOrDefaultAdaptor(size_t position):
			position(position)
		{}

		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
			requires std::default_initializable<TRangeValue<TRange>>
		{
			if(std::ranges::size(range) <= position)
			{
				return TRangeValue<TRange>{};
			}
			else
			{
				auto it = range.begin();
				std::ranges::advance(it, position);
				return *it;
			}
		}
	};

	constexpr auto ElementAtOrDefault(size_t position)
	{
		return _ElementAtOrDefaultAdaptor(position);
	}

	template<typename T>
	constexpr auto Empty()
	{
		return std::views::empty<T>;
	}

	// TODO: Except
	// TODO: ExceptBy

	struct _FirstAdaptor: public RangeAdaptor<_FirstAdaptor>
	{
		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			return *range.begin();
		}
	};

	constexpr auto Fisrt()
	{
		return _FirstAdaptor();
	}

	struct _FirstOrDefaultAdaptor: public RangeAdaptor<_FirstOrDefaultAdaptor>
	{
		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
			requires std::default_initializable<TRangeValue<TRange>>
		{
			if(range.begin() == range.end())
			{
				return TRangeValue<TRange>{};
			}

			return *range.begin();
		}
	};

	template<typename T>
	struct _FirstOrDefaultAdaptor2: public RangeAdaptor<_FirstOrDefaultAdaptor2<T>>
	{
		T defaultValue;

		constexpr _FirstOrDefaultAdaptor2(const T& defaultValue):
			defaultValue(defaultValue)
		{}

		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			if(range.begin() == range.end())
			{
				return defaultValue;
			}

			return *range.begin();
		}
	};

	constexpr auto FirstOrDefault()
	{
		return _FirstOrDefaultAdaptor();
	}

	template<typename T>
	constexpr auto FirstOrDefault(const T& defaultValue)
	{
		return _FirstOrDefaultAdaptor2<T>(defaultValue);
	}

	template<typename TPredicate>
	struct _FindFirstAdaptor: public RangeAdaptor<_FindFirstAdaptor<TPredicate>>
	{
		TPredicate predicate;

		constexpr _FindFirstAdaptor(TPredicate&& predicate):
			predicate(std::move(predicate))
		{}

		template<std::ranges::range TRange>
		constexpr std::optional<TRangeValue<TRange>> operator()(TRange&& range) const
		{
			for(const auto& item : range)
			{
				if(predicate(item))
				{
					return std::optional<TRangeValue<TRange>>(item);
				}
			}

			return std::nullopt;
		}
	};

	template<typename TPredicate>
	constexpr auto FindFirst(TPredicate predicate)
	{
		return _FindFirstAdaptor<TPredicate>(std::move(predicate));
	}

	template<typename TPredicate, typename TDefaultValue>
	struct _FindFirstOrDefaultAdaptor: public RangeAdaptor<_FindFirstOrDefaultAdaptor<TPredicate, TDefaultValue>>
	{
		TPredicate predicate;
		TDefaultValue defaultValue;

		constexpr _FindFirstOrDefaultAdaptor(TPredicate&& predicate, const TDefaultValue& defaultValue):
			predicate(std::move(predicate)),
			defaultValue(defaultValue)
		{}

		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			for(const auto& item : range)
			{
				if(predicate(item))
				{
					return item;
				}
			}

			return defaultValue;
		}
	};

	template<typename TPredicate, typename TDefaultValue>
	constexpr auto FindFirstOrDefault(TPredicate predicate, const TDefaultValue& defaultValue)
	{
		return _FindFirstOrDefaultAdaptor<TPredicate, TDefaultValue>(std::move(predicate), defaultValue);
	}

	template<size_t size>
	struct _ToArrayAdaptor: public RangeAdaptor<_ToArrayAdaptor<size>>
	{
		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			const auto minSize = std::min(std::ranges::size(range), size);
			std::array<TRangeValue<TRange>, size> result = {};
			auto it = range.begin();

			for(size_t i = 0; i < minSize; i++, it++)
			{
				result[i] = *it;
			}

			return result;
		}
	};

	template<size_t size>
	constexpr auto ToArray()
	{
		return _ToArrayAdaptor<size>();
	}

	struct _ToVectorAdaptor: public RangeAdaptor<_ToVectorAdaptor>
	{
		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			return std::vector(range.begin(), range.end());
		}
	};

	constexpr auto ToVector()
	{
		return _ToVectorAdaptor();
	}
}