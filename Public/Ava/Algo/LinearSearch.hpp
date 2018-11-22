#pragma once

#include "Ava/Meta/Identity.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Utility/ConstParam.hpp"
#include "Ava/Utility/Forward.hpp"
#include "Ava/Utility/Range.hpp"

namespace Ava::Private::Algo_LinearSearch {

template<typename TPredicate, typename TIterator>
TIterator LinearSearch(TIterator first, TIterator last, ConstParam<TPredicate> predicate)
{
	for (; first != last; ++first)
		if (predicate(*first))
			return first;

	return last;
}

} // namespace Ava::Private::Algo_LinearSearch

namespace Ava::Algo {

template<typename TIterator, typename TPredicate>
Ava_FORCEINLINE auto LinearSearch(TIterator first, TIterator last, const TPredicate& predicate)
	-> Identity<TIterator, decltype(predicate(*first))>
{
	return Ava::Private::Algo_LinearSearch
		::LinearSearch<TPredicate>(first, last, predicate);
}

template<typename TIterator, typename T>
Ava_FORCEINLINE auto LinearSearch(TIterator first, TIterator last, const T& value)
	-> Identity<TIterator, decltype(*first == value)>
{
	return LinearSearch(first, last,
		[v = ConstParamWrapper(value)](const auto& x) { return x == v.Value; });
}

template<typename TIterator, typename TPredicate>
Ava_FORCEINLINE bool Any(TIterator first, TIterator last, const TPredicate& predicate)
{
	return LinearSearch(first, last, predicate) != last;
}

template<typename TRange, typename TPredicate>
Ava_FORCEINLINE bool Any(const TRange& range, const TPredicate& predicate)
{
	return Any(Begin(range), End(range), predicate);
}

template<typename TIterator, typename TPredicate>
Ava_FORCEINLINE bool All(TIterator first, TIterator last, const TPredicate& predicate)
{
	return LinearSearch(first, last,
		[p = ConstParamWrapper(predicate)](const auto& x) { return !p.Value(x); }) == last;
}

template<typename TRange, typename TPredicate>
Ava_FORCEINLINE bool All(const TRange& range, const TPredicate& predicate)
{
	return All(Begin(range), End(range), predicate);
}

template<typename TIterator, typename TPredicate>
Ava_FORCEINLINE bool None(TIterator first, TIterator last, const TPredicate& predicate)
{
	return LinearSearch(first, last,
		[p = ConstParamWrapper(predicate)](const auto& x) { return p.Value(x); }) == last;
}

template<typename TRange, typename TPredicate>
Ava_FORCEINLINE bool None(const TRange& range, const TPredicate& predicate)
{
	return None(Begin(range), End(range), predicate);
}

} // namespace Ava::Algo
