#pragma once

#include <ranges>
#include <algorithm>
#include <functional>

namespace Sgl::Ranges
{
	template<typename TAdaptor>
	struct RangeAdaptor
	{
		constexpr auto operator()(std::ranges::range auto&& range) const
		{
			return static_cast<const TAdaptor&>(*this)(std::forward<decltype(range)>(range));
		}

		constexpr friend decltype(auto) operator|(std::ranges::range auto&& range,
												  RangeAdaptor&& adaptor)
		{
			return adaptor(std::forward<decltype(range)>(range));
		}
	};

	template<typename TFunc>
	struct _AggregateAdaptor: public RangeAdaptor<_AggregateAdaptor<TFunc>>
	{
		TFunc Function;

		constexpr _AggregateAdaptor(TFunc&& func):
			Function(std::forward<TFunc>(func)) {}

		constexpr auto operator()(std::ranges::range auto&& range) const
		{
			std::ranges::range_value_t<decltype(range)> result = {};
			for(const auto& item : range)
			{
				result = Function(result, item);
			}

			return result;
		}
	};

	template<typename TFunc, typename TAccumulate>
	struct _AgregateAdaptor2: public RangeAdaptor<_AgregateAdaptor2<TFunc, TAccumulate>>
	{
		TFunc Function;
		TAccumulate Seed;

		constexpr _AgregateAdaptor2(TFunc&& func, TAccumulate&& seed):
			Function(std::forward<TFunc>(func)),
			Seed(std::forward<TAccumulate>(seed))
		{}

		constexpr TAccumulate operator()(std::ranges::range auto&& range) const
		{
			TAccumulate result = Seed;

			for(const auto& item : range)
			{
				result = Function(result, item);
			}

			return result;
		}
	};

	template<typename TFunc>
	inline constexpr auto Aggregate(TFunc&& func)
	{
		return _AggregateAdaptor<TFunc>(std::forward<TFunc>(func));
	}

	template<typename TFunc, typename TAccumulate>
	inline constexpr auto Aggregate(TFunc&& func, TAccumulate&& seed)
	{
		return _AgregateAdaptor2<TFunc, TAccumulate>(
			std::forward<TFunc>(func), std::forward<TAccumulate>(seed));
	}

	template<typename TPredicate>
	struct _AllAdaptor: public RangeAdaptor<_AllAdaptor<TPredicate>>
	{
		TPredicate Predicate;

		constexpr _AllAdaptor(TPredicate&& predicate):
			Predicate(std::forward<TPredicate>(predicate)) {}

		constexpr bool operator()(std::ranges::range auto&& range) const
		{
			for(const auto& item : range)
			{
				if(!Predicate(item))
				{
					return false;
				}
			}

			return true;
		}
	};

	template<typename TPredicate>
	inline constexpr auto All(TPredicate&& predicate)
	{
		return _AllAdaptor<TPredicate>(std::forward<TPredicate>(predicate));
	}

	template<typename TPredicate>
	struct _AnyAdaptor: public RangeAdaptor<_AnyAdaptor<TPredicate>>
	{
		TPredicate Predicate;

		constexpr _AnyAdaptor(TPredicate&& predicate):
			Predicate(std::forward<TPredicate>(predicate)) {}

		constexpr bool operator()(std::ranges::range auto&& range) const
		{
			for(const auto& item : range)
			{
				if(Predicate(item))
				{
					return true;
				}
			}

			return false;
		}
	};

	template<typename TPredicate>
	inline constexpr auto Any(TPredicate&& predicate)
	{
		return _AnyAdaptor<TPredicate>(std::forward<TPredicate>(predicate));
	}

	struct _AverageAdaptor: public RangeAdaptor<_AverageAdaptor>
	{		
		constexpr double operator()(std::ranges::range auto&& range) const
		{
			double sum = 0;

			for(const auto& item : range)
			{
				sum += item;
			}
			
			return sum / std::ranges::distance(range);
		}
	};

	inline constexpr auto Average()
	{
		return _AverageAdaptor();
	}

	template<typename TResult>
	struct _CastAdaptor: public RangeAdaptor<_CastAdaptor<TResult>>
	{
		constexpr auto operator()(std::ranges::range auto&& range) const
		{
			return range | std::views::transform([](const auto& item)
			{
				return static_cast<TResult>(item);
			});
		}
	};

	template<typename TResult>
	inline constexpr auto Cast()
	{
		return _CastAdaptor<TResult>();
	}

	struct _ChunkAdaptor: public RangeAdaptor<_ChunkAdaptor>
	{
		size_t Size;

		constexpr _ChunkAdaptor(size_t size): Size(size) {}

		constexpr auto operator()(std::ranges::range auto&& range) const
		{
			if(Size == 0)
			{
				throw std::invalid_argument("Chunk size must be more than 0");
			}

			const auto RangeSize = std::ranges::distance(range);
			const auto ChunksNumber = RangeSize / Size;
			std::vector<std::vector<std::ranges::range_value_t<decltype(range)>>> chunks(ChunksNumber);
			auto it = range.begin();

			for(auto& chunk : chunks)
			{
				auto c = std::views::counted(it, Size);
				chunk = std::vector(c.begin(), c.end());
				it += Size;
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

	inline constexpr auto Chunk(size_t size)
	{
		return _ChunkAdaptor(size);
	}

	struct _CountAdaptor: public RangeAdaptor<_CountAdaptor>
	{
		constexpr auto operator()(std::ranges::range auto&& range) const
		{
			return std::ranges::distance(range);
		}
	};

	inline constexpr auto Count()
	{
		return _CountAdaptor();
	}

	struct _ToVectorAdaptor: public RangeAdaptor<_ToVectorAdaptor>
	{
		constexpr auto operator()(std::ranges::range auto&& range) const
		{
			return std::vector(range.begin(), range.end());
		}
	};

	inline constexpr auto ToVector()
	{
		return _ToVectorAdaptor();
	}
}