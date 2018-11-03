#pragma once

#include "Ava/Algo/Utility.hpp"
#include "Ava/Memory/AlignedStorage.hpp"
#include "Ava/Memory/Utility.hpp"
#include "Ava/Meta/Traits.hpp"
#include "Ava/Misc.hpp"
#include "Ava/Types.hpp"
#include "Ava/Utility/Forward.hpp"

namespace Ava {

namespace Private::Memory_Storage {

template<typename T>
class StorageLayer
{
public:
	Ava_FORCEINLINE void Construct()
	{
		ValueConstruct<T>(Get());
	}

	template<typename... TArgs>
	Ava_FORCEINLINE void Construct(TArgs&&... args)
	{
		Ava::Construct<T>(Get(), Forward<TArgs>(args)...);
	}

	Ava_FORCEINLINE void Destroy()
	{
		Ava::Destroy<T>(Get());
	}

protected:
	T* Get()
	{
		return (T*)&m_storage;
	}

	const T* Get() const
	{
		return (const T*)&m_storage;
	}

private:
	AlignedStorage<
		sizeof(T), alignof(T)
	> m_storage;
};

template<typename T, uword TSize>
class StorageLayer<T[TSize]>
{
public:
	Ava_FORCEINLINE void Construct()
	{
		if constexpr (TSize == 1)
		{
			ValueConstruct<T>(Get());
		}
		else
		{
			Algo::ValueConstruct(*Get(), TSize);
		}
	}

	template<typename... TArgs>
	Ava_FORCEINLINE void Construct(TArgs&&... args)
	{
		if constexpr (TSize == 1)
		{
			Ava::Construct<T>(Get(), Forward<TArgs>(args)...);
		}
		else
		{
			Algo::Construct(*Get(), TSize, args...);
		}
	}

	Ava_FORCEINLINE void Destroy()
	{
		if constexpr (TSize == 1)
		{
			Ava::Destroy<T>(*Get());
		}
		else
		{
			Algo::Destroy(*Get(), TSize);
		}
	}

protected:
	T (*Get())[TSize]
	{
		return (T(*)[TSize])&m_storage;
	}

	const T (*Get() const)[TSize]
	{
		return (const T(*)[TSize])&m_storage;
	}

private:
	AlignedStorage<
		sizeof(T) * TSize, alignof(T)
	> m_storage;
};

template<typename T, bool>
struct DestroyLayer;

template<typename T>
struct DestroyLayer<T, 0> : StorageLayer<T>
{
};

template<typename T>
struct DestroyLayer<T, 1> : StorageLayer<T>
{
	Ava_FORCEINLINE ~DestroyLayer()
	{
		StorageLayer<T>::Destroy();
	}
};

} // namespace Private::Memory_Storage

namespace StorageOptions {

enum : u32
{
	Construct = 1 << 0,
	Destroy = 1 << 1,
};

} // namespace StorageOptions

template<typename T, u32 TOptions = 0>
class Storage final : public Private::Memory_Storage::DestroyLayer<
	T, (TOptions & StorageOptions::Destroy) && !IsTriviallyDestructible<T>>
{
	// Intentionally skip the destroy layer
	typedef Private::Memory_Storage::StorageLayer<T> Base;

	static constexpr u32 AllFlags =
		StorageOptions::Construct |
		StorageOptions::Destroy;

	// Only the valid flag bits may be set
	static_assert((TOptions & ~AllFlags) == 0);

public:
	Ava_FORCEINLINE Storage()
	{
		if constexpr (TOptions & StorageOptions::Construct)
			Base::Construct();
	}

	Storage(Storage&&) = delete;
	Storage(const Storage&) = delete;
	Storage& operator=(Storage&&) = delete;
	Storage& operator=(const Storage&) = delete;

	Ava_FORCEINLINE T& operator*()
	{
		return *Base::Get();
	}

	Ava_FORCEINLINE const T& operator*() const
	{
		return *Base::Get();
	}

	Ava_FORCEINLINE T* operator->()
	{
		return Base::Get();
	}

	Ava_FORCEINLINE const T* operator->() const
	{
		return Base::Get();
	}

	Ava_FORCEINLINE T* operator&()
	{
		return Base::Get();
	}

	Ava_FORCEINLINE const T* operator&() const
	{
		return Base::Get();
	}
};

} // namespace Ava
