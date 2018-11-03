static Ava_PLATFORM_FORCEINLINE u32 Lzcnt(u32 x)
{
	return _lzcnt_u32(x);
}

static Ava_PLATFORM_FORCEINLINE u64 Lzcnt(u64 x)
{
#if Ava_X64
	return _lzcnt_u64(x);
#else
	u32 h = _lzcnt_u32((u32)(x >> 32));
	u32 l = _lzcnt_u32((u32)x);
	return h != 32 ? h : l;
#endif
}

static Ava_PLATFORM_FORCEINLINE u32 Tzcnt(u32 x)
{
	return _tzcnt_u32(x);
}

static Ava_PLATFORM_FORCEINLINE u64 Tznct(u64 x)
{
#if Ava_X64
	return _tzcnt_u64(x);
#else
	u32 h = _tzcnt_u32((u32)(x >> 32));
	u32 l = _tzcnt_u32((u32)x);
	return l != 32 ? l : h;
#endif
}
