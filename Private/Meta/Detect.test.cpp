#include "Ava/Meta/Detect.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Meta/ValueOf.hpp"

using namespace Ava;

template<typename T>
using Dereference = decltype(*ValueOf<T>);

static_assert(Detect<Dereference, int*>::Value);
static_assert(IsSame<typename Detect<Dereference, int*>::Type, int&>);
static_assert(IsSame<typename Detect<Dereference, int*>::template Default<int>, int&>);

static_assert(!Detect<Dereference, int>::Value);
static_assert(IsSame<typename Detect<Dereference, int>::template Default<int>, int>);
