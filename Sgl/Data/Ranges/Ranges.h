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
		TFunc Function;

		constexpr _AggregateAdaptor(TFunc&& func):
			Function(std::move(func)) 
		{}

		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			TRangeValue<TRange> result = {};
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

		constexpr _AgregateAdaptor2(TFunc&& func, const TAccumulate& seed):
			Function(std::move(func)),
			Seed(seed)
		{}

		template<std::ranges::range TRange>
		constexpr TAccumulate operator()(TRange&& range) const
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
		TPredicate Predicate;

		constexpr _AllAdaptor(TPredicate&& predicate):
			Predicate(std::move(predicate)) 
		{}

		template<std::ranges::range TRange>
		constexpr bool operator()(TRange&& range) const
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
	constexpr auto All(TPredicate predicate)
	{
		return _AllAdaptor<TPredicate>(std::move(predicate));
	}

	template<typename TPredicate>
	struct _AnyAdaptor: public RangeAdaptor<_AnyAdaptor<TPredicate>>
	{
		TPredicate Predicate;

		constexpr _AnyAdaptor(TPredicate&& predicate):
			Predicate(std::move(predicate))
		{}

		template<std::ranges::range TRange>
		constexpr bool operator()(TRange&& range) const
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
			
			return sum / std::ranges::distance(range);
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

	struct _CountAdaptor: public RangeAdaptor<_CountAdaptor>
	{
		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			return std::ranges::distance(range);
		}
	};

	constexpr auto Count()
	{
		return _CountAdaptor();
	}

	template<typename T>
	struct _ContainsAdaptor: public RangeAdaptor<_ContainsAdaptor<T>>
	{
		T Value;

		constexpr _ContainsAdaptor(const T& value):
			Value(value)
		{}

		template<std::ranges::range TRange>
		constexpr bool operator()(TRange&& range) const
			requires std::convertible_to<TRangeValue<TRange>, T>
		{
			for(const auto& item : range)
			{
				if(item == Value)
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

	struct _ElementAtAdaptor: public RangeAdaptor<_ElementAtAdaptor>
	{
		size_t Position;

		constexpr _ElementAtAdaptor(size_t position):
			Position(position)
		{}

		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			auto it = range.begin();
			std::ranges::advance(it, Position);
			return *it;
		}
	};

	constexpr auto ElementAt(size_t position)
	{
		return _ElementAtAdaptor(position);
	}

	template<typename T>
	constexpr auto Empty()
	{
		return std::views::empty<T>;
	}

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

	template<typename TPredicate>
	struct _FindAdaptor: public RangeAdaptor<_FindAdaptor<TPredicate>>
	{
		TPredicate Predicate;

		constexpr _FindAdaptor(TPredicate predicate):
			Predicate(std::move(predicate))
		{}

		template<std::ranges::range TRange>
		constexpr std::optional<TRangeValue<TRange>> operator()(TRange&& range) const
		{
			for(const auto& item : range)
			{
				if(Predicate(item))
				{
					return std::optional<TRangeValue<TRange>>(item);
				}
			}

			return std::nullopt;
		}
	};

	template<typename TPredicate>
	constexpr auto Find(TPredicate predicate)
	{
		return _FindAdaptor<TPredicate>(std::move(predicate));
	}

	struct _LastAdaptor: public RangeAdaptor<_LastAdaptor>
	{
		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			return *std::prev(range.end());
		}
	};

	constexpr auto Last()
	{
		return _LastAdaptor();
	}

	struct _MaxAdaptor: public RangeAdaptor<_MaxAdaptor>
	{
		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			return std::ranges::max(std::forward<TRange>(range));
		}
	};

	template<typename TProjection>
	struct _MaxByAdaptor: public RangeAdaptor<_MaxByAdaptor<TProjection>>
	{
		TProjection Projection;

		_MaxByAdaptor(TProjection projection):
			Projection(projection)
		{}

		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			return std::ranges::max(std::forward<TRange>(range), {}, Projection);
		}
	};

	constexpr auto Max()
	{
		return _MaxAdaptor();
	}

	template<typename TProjection>
	constexpr auto MaxBy(TProjection projection)
	{
		return _MaxByAdaptor<TProjection>(projection);
	}

	struct _MinAdaptor: public RangeAdaptor<_MinAdaptor>
	{
		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			return std::ranges::min(std::forward<TRange>(range));
		}
	};

	template<typename TProjection>
	struct _MinByAdaptor: public RangeAdaptor<_MinByAdaptor<TProjection>>
	{
		TProjection Projection;

		_MinByAdaptor(TProjection projection):
			Projection(projection)
		{}

		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			return std::ranges::min(std::forward<TRange>(range), {}, Projection);
		}
	};

	constexpr auto Min()
	{
		return _MinAdaptor();
	}

	template<typename TProjection>
	constexpr auto MinBy(TProjection projection)
	{
		return _MinByAdaptor<TProjection>(projection);
	}

	template<std::ranges::view TRange>
	class OrderedView: public std::ranges::view_interface<OrderedView<TRange>>
	{
	private:
		TRange _range;
		std::optional<std::vector<TRangeValue<TRange>>> _ordered;
	public:
		OrderedView(const OrderedView&) requires std::copyable<TRange> = default;
		OrderedView(OrderedView&&) = default;

		constexpr explicit OrderedView(TRange&& range) noexcept:
			_range(std::move(range))
		{}

		constexpr auto begin() noexcept
		{
			TrySort();
			return _ordered->begin();
		}

		constexpr auto end() noexcept
		{
			TrySort();
			return _ordered->end();
		}

		OrderedView& operator=(const OrderedView&) requires std::copyable<TRange> = default;
		OrderedView& operator=(OrderedView&&) = default;
	private:
		constexpr void TrySort()
		{
			if(!_ordered)
			{
				_ordered.emplace(std::ranges::begin(_range), std::ranges::end(_range));
				std::ranges::sort(*_ordered);
			}
		}
	};

	struct _OrderAdaptor: public RangeAdaptor<_OrderAdaptor>
	{
		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			if constexpr(std::ranges::view<TRange>)
			{
				return OrderedView(std::forward<TRange>(range));
			}
			else
			{
				return OrderedView(std::views::all(std::forward<TRange>(range)));
			}
		}
	};

	constexpr auto Order()
	{
		return _OrderAdaptor();
	}

	constexpr auto Range(int start, int count)
	{
		return std::views::iota(start, count);
	}

	template<typename TSelector>
	constexpr auto Select(TSelector&& selector)
	{
		return std::views::transform(std::forward<TSelector>(selector));
	}

	constexpr auto Slice(size_t start, size_t count)
	{
		return std::views::drop(start) | std::views::take(count);
	}

	template<size_t Size>
	struct _ToArrayAdaptor: public RangeAdaptor<_ToArrayAdaptor<Size>>
	{
		template<std::ranges::range TRange>
		constexpr auto operator()(TRange&& range) const
		{
			const auto minSize = std::min(static_cast<size_t>(std::ranges::distance(range)), Size);
			auto result = std::array<TRangeValue<TRange>, Size>();
			auto it = range.begin();

			for(size_t i = 0; i < minSize; i++, it++)
			{
				result[i] = *it;
			}

			return result;
		}
	};

	template<size_t Size>
	constexpr auto ToArray()
	{
		return _ToArrayAdaptor<Size>();
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

	template<typename TPredicate>
	constexpr auto Where(TPredicate&& predicate)
	{
		return std::views::filter(std::forward<TPredicate>(predicate));
	}
}