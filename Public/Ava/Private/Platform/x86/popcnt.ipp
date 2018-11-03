static Ava_PLATFORM_FORCEINLINE i32 Popcnt(u32 x)
{
	return _mm_popcnt_u32(x);
}

static Ava_PLATFORM_FORCEINLINE i32 Popcnt(u64 x)
{
#if Ava_X64
	return (i32)_mm_popcnt_u64(x);
#else
	return _mm_popcnt_u32((u32)x) + _mm_popcnt_u32((u32)(x >> 32));
#endif
}
