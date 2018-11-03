#pragma once

#include "Ava/Memory/Storage.hpp"
#include "Ava/Meta/Traits.hpp"
#include "Ava/Meta/TypeTraits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Utility/Move.hpp"

namespace Ava {

namespace Private::Utility_Maybe {

template<uword>
struct Tombstone
{
	typedef uword Type;
};

template<>
struct Tombstone<1>
{
	typedef u8 Type;
};

template<>
struct Tombstone<2>
{
	typedef u16 Type;
};

template<>
struct Tombstone<4>
{
	typedef u32 Type;
};

template<typename T, typename TTombstoneTraits, bool = TTombstoneTraits::Count != 0>
struct StorageLayer;

template<typename T, typename TTombstoneTraits>
struct StorageLayer<T, TTombstoneTraits, 0>
{
	typedef typename Tombstone<alignof(T)>::Type TombstoneType;
	static constexpr uword TombstoneCount = ~(TombstoneType)0;

	static Ava_FORCEINLINE uword GetTombstone(const StorageLayer* ptr)
	{
		return ptr->m_tombstone;
	}

	static Ava_FORCEINLINE void SetTombstone(StorageLayer* ptr, uword index)
	{
		ptr->m_tombstone = (TombstoneType)index;
	}

	Ava_FORCEINLINE bool HasTombstone() const
	{
		return GetTombstone(this) == 0;
	}

	Ava_FORCEINLINE void SetTombstone()
	{
		SetTombstone(this, 0);
	}

	Ava_FORCEINLINE void ResetTombstone()
	{
		SetTombstone(this, (uword)-1);
	}

	Storage<T> m_storage;
	TombstoneType m_tombstone;
};

template<typename T, typename TTombstoneTraits>
struct StorageLayer<T, TTombstoneTraits, 1>
{
	static constexpr uword TombstoneCount = TTombstoneTraits::Count;

	static Ava_FORCEINLINE uword GetTombstone(const StorageLayer* ptr)
	{
		return TTombstoneTraits::GetIndex(&ptr->m_storage);
	}

	static Ava_FORCEINLINE void SetTombstone(StorageLayer* ptr, uword index)
	{
		TTombstoneTraits::SetIndex(&ptr->m_storage, index);
	}

	Ava_FORCEINLINE bool HasTombstone() const
	{
		return GetTombstone(this) < TTombstoneTraits::Count;
	}

	Ava_FORCEINLINE void SetTombstone()
	{
		SetTombstone(this, 0);
	}

	Ava_FORCEINLINE void ResetTombstone()
	{
	}

	Storage<T> m_storage;
};

template<typename T, typename TTombstoneTraits, bool = IsTriviallyDestructible<T>>
struct DestroyLayer;

template<typename T, typename TTombstoneTraits>
struct DestroyLayer<T, TTombstoneTraits, 0> : StorageLayer<T, TTombstoneTraits>
{
	~DestroyLayer()
	{
		typedef StorageLayer<T, TTombstoneTraits> Base;

		if (!Base::HasTombstone())
			Base::m_storage.Destroy();
	}
};

template<typename T, typename TTombstoneTraits>
struct DestroyLayer<T, TTombstoneTraits, 1> : StorageLayer<T, TTombstoneTraits>
{
};

enum class MaybeNot {};

} // namespace Private::Utility_Maybe

constexpr Private::Utility_Maybe::MaybeNot MaybeNot = {};

template<typename T, typename TTombstoneTraits = TombstoneTraits<T>>
class Maybe : Private::Utility_Maybe::DestroyLayer<T, TTombstoneTraits>
{
	typedef Private::Utility_Maybe::StorageLayer<T, TTombstoneTraits> Base;

public:
	typedef T ValueType;

	Ava_FORCEINLINE Maybe()
	{
		Base::SetTombstone();
	}

	Ava_FORCEINLINE Maybe(decltype(MaybeNot))
	{
		Base::SetTombstone();
	}

	template<typename TIn, EnableIf<!IsSame<Decay<TIn>, Maybe> && IsConvertibleTo<TIn, T>, int> = 0>
	Ava_FORCEINLINE Maybe(TIn&& value)
	{
		Base::ResetTombstone();
		Base::m_storage.Construct(Forward<TIn>(value));
	}

	template<typename TIn, EnableIf<!IsSame<Decay<TIn>, Maybe> && !IsConvertibleTo<TIn, T>, int> = 0>
	explicit Ava_FORCEINLINE Maybe(TIn&& value)
	{
		Base::ResetTombstone();
		Base::m_storage.Construct(Forward<TIn>(value));
	}

	Ava_FORCEINLINE Maybe(Maybe&& other)
	{
		if constexpr (IsTriviallyRelocatable<T>)
		{
			Memory::Copy(this, &other, sizeof(*this));
			other.SetTombstone();
		}
		else
		{
			if (other.HasTombstone())
			{
				Base::SetTombstone();
			}
			else
			{
				Base::ResetTombstone();
				Relocate<T>(&this->Base::m_storage, &other.m_storage);
				other.SetTombstone();
			}
		}
	}

	Ava_FORCEINLINE Maybe(const Maybe& other)
	{
		if constexpr (IsTriviallyCopyable<T>)
		{
			Memory::Copy(this, &other, sizeof(*this));
		}
		else
		{
			if (other.HasTombstone())
			{
				Base::SetTombstone();
			}
			else
			{
				Base::ResetTombstone();
				Base::m_storage.Construct(*other.m_storage);
			}
		}
	}

	template<typename TInTraits>
	Ava_FORCEINLINE Maybe(Maybe<T, TInTraits>&& other)
	{
		if (other.HasTombstone())
		{
			Base::SetTombstone();
		}
		else
		{
			Base::ResetTombstone();
			Relocate<T>(&this->Base::m_storage, &other.m_storage);
			other.SetTombstone();
		}
	}

#define Ava_FUNC_BODY \
	if (other.HasTombstone()) { \
		Base::SetTombstone(); } \
	else { \
		Base::ResetTombstone(); \
		Base::m_storage.Construct(Move(*other.m_storage)); \
		other.m_storage.Destroy(); \
		other.SetTombstone(); }

	template<typename TIn, typename TInTraits, EnableIf<IsConvertibleTo<TIn, T>, int> = 0>
	Ava_FORCEINLINE Maybe(Maybe<TIn, TInTraits>&& other) { Ava_FUNC_BODY }

	template<typename TIn, typename TInTraits, EnableIf<!IsConvertibleTo<TIn, T>, int> = 0>
	explicit Ava_FORCEINLINE Maybe(Maybe<TIn, TInTraits>&& other) { Ava_FUNC_BODY }

#undef Ava_FUNC_BODY
#define Ava_FUNC_BODY \
	if (other.HasTombstone()) { \
		Base::SetTombstone(); } \
	else { \
		Base::ResetTombstone(); \
		Base::m_storage.Construct(*other.m_storage); }

	template<typename TIn, typename TInTraits, EnableIf<IsConvertibleTo<TIn, T>, int> = 0>
	Ava_FORCEINLINE Maybe(const Maybe<TIn, TInTraits>& other) { Ava_FUNC_BODY }

	template<typename TIn, typename TInTraits, EnableIf<!IsConvertibleTo<TIn, T>, int> = 0>
	explicit Ava_FORCEINLINE Maybe(const Maybe<TIn, TInTraits>& other) { Ava_FUNC_BODY }

#undef Ava_FUNC_BODY

	Ava_FORCEINLINE Maybe& operator=(decltype(MaybeNot))
	{
		Reset();

		return *this;
	}

	Ava_FORCEINLINE Maybe& operator=(Maybe&& other)
	{
		if constexpr (IsTriviallyRelocatable<T>)
		{
			Memory::Copy(this, &other, sizeof(*this));
			other.SetTombstone();
		}
		else
		{
			if constexpr (!IsTriviallyDestructible<T>)
				if (!Base::HasTombstone())
					Base::m_storage.Destroy();

			if (other.HasTombstone())
			{
				Base::SetTombstone();
			}
			else
			{
				Base::ResetTombstone();
				Relocate<T>(&this->Base::m_storage, &other.m_storage);
				other.SetTombstone();
			}
		}

		return *this;
	}

	Ava_FORCEINLINE Maybe& operator=(const Maybe& other)
	{
		if constexpr (IsTriviallyCopyable<T>)
		{
			Memory::Copy(this, &other, sizeof(*this));
		}
		else
		{
			if constexpr (!IsTriviallyDestructible<T>)
				if (!Base::HasTombstone())
					Base::m_storage.Destroy();

			if (other.HasTombstone())
			{
				Base::SetTombstone();
			}
			else
			{
				Base::ResetTombstone();
				CopyConstruct<T>(&this->Base::m_storage, *other.m_storage);
			}
		}

		return *this;
	}

#define Ava_SFINAE(...) EnableIf<IsConvertibleTo<TIn, T>, __VA_ARGS__>

	template<typename TIn, typename TInTraits>
	Ava_FORCEINLINE Ava_SFINAE(Maybe&) operator=(Maybe<TIn, TInTraits>&& other)
	{
		if constexpr (!IsTriviallyDestructible<T>)
			if (!Base::HasTombstone())
				Base::m_storage.Destroy();

		if (other.HasTombstone())
		{
			Base::SetTombstone();
		}
		else
		{
			Base::ResetTombstone();
			Construct<T>(&this->Base::m_storage, Move(*other.m_storage));
			other.m_storage.Destroy();
			other.SetTombstone();
		}

		return *this;
	}

	template<typename TIn, typename TInTraits>
	Ava_FORCEINLINE Ava_SFINAE(Maybe&) operator=(const Maybe<TIn, TInTraits>& other)
	{
		if constexpr (!IsTriviallyDestructible<T>)
			if (!Base::HasTombstone())
				Base::m_storage.Destroy();

		if (other.HasTombstone())
		{
			Base::SetTombstone();
		}
		else
		{
			Base::ResetTombstone();
			Construct<T>(&this->Base::m_storage, *other.m_storage);
			other.SetTombstone();
		}

		return *this;
	}

#undef Ava_SFINAE
#define Ava_SFINAE(...) EnableIf<IsConvertibleTo<Decay<TIn>, T>, __VA_ARGS__>

	template<typename TIn>
	Ava_FORCEINLINE Ava_SFINAE(Maybe&) operator=(TIn&& value)
	{
		if (Base::HasTombstone())
		{
			Base::ResetTombstone();
		}
		else
		{
			Base::m_storage.Destroy();
		}

		Base::m_storage.Construct(Forward<TIn>(value));

		return *this;
	}

#undef Ava_SFINAE

	Ava_FORCEINLINE T* operator->()
	{
		Ava_Assert(!Base::HasTombstone());
		return &this->Base::m_storage;
	}

	Ava_FORCEINLINE const T* operator->() const
	{
		Ava_Assert(!Base::HasTombstone());
		return &this->Base::m_storage;
	}

	Ava_FORCEINLINE T& operator*() &
	{
		Ava_Assert(!Base::HasTombstone());
		return *Base::m_storage;
	}

	Ava_FORCEINLINE const T& operator*() const &
	{
		Ava_Assert(!Base::HasTombstone());
		return *Base::m_storage;
	}

	Ava_FORCEINLINE T&& operator*() &&
	{
		Ava_Assert(!Base::HasTombstone());
		return Move(*Base::m_storage);
	}

	Ava_FORCEINLINE const T&& operator*() const &&
	{
		Ava_Assert(!Base::HasTombstone());
		return Move(*Base::m_storage);
	}

	Ava_FORCEINLINE bool HasValue() const
	{
		return !Base::HasTombstone();
	}

	explicit Ava_FORCEINLINE operator bool() const
	{
		return !Base::HasTombstone();
	}

	template<typename... TArgs>
	Ava_FORCEINLINE void Emplace(TArgs&&... args)
	{
		if (Base::HasTombstone())
		{
			Base::ResetTombstone();
		}
		else
		{
			Base::m_storage.Destroy();
		}

		Base::m_storage.Construct(Forward<TArgs>(args)...);
	}

	Ava_FORCEINLINE void Reset()
	{
		if (!Base::HasTombstone())
		{
			Base::m_storage.Destroy();
			Base::SetTombstone();
		}
	}

	Ava_FORCEINLINE T Extract()
	{
		Ava_Assert(!Base::HasTombstone());

		Storage<T, StorageOptions::Destroy> storage;
		Relocate<T>(&storage, &this->Base::m_storage);
		Base::SetTombstone();

		return Move(*storage);
	}

	template<typename, typename>
	friend class Maybe;

	friend struct TombstoneTraits<Maybe<T, TombstoneTraits<T>>>;
};

template<typename T, typename TTombstoneTraits>
struct TombstoneTraits<Maybe<T, TTombstoneTraits>>
{
private:
	typedef Maybe<T, TTombstoneTraits> MaybeType;

public:
	static constexpr uword Count = MaybeType::TombstoneCount - 1;

	static Ava_FORCEINLINE uword GetIndex(const MaybeType* maybe)
	{
		return MaybeType::GetTombstone(maybe) - 1;
	}

	static Ava_FORCEINLINE void SetIndex(MaybeType* maybe, uword index)
	{
		MaybeType::SetTombstone(maybe, index + 1);
	}
};

template<typename T, typename TTombstoneTraits>
constexpr bool IsTriviallyRelocatable<Maybe<T, TTombstoneTraits>> = IsTriviallyRelocatable<T>;

template<typename T, typename TTombstoneTraits>
constexpr bool IsTriviallyDestructible<Maybe<T, TTombstoneTraits>> = IsTriviallyDestructible<T>;

template<typename T, typename TTombstoneTraits>
constexpr bool IsZeroConstructible<Maybe<T, TTombstoneTraits>> = TTombstoneTraits::Count == 0;

} // namespace Ava
