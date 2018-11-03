static Ava_PLATFORM_FORCEINLINE void* Copy(void* dst, const void* src, uword size)
{
	return ::memcpy(dst, src, size);
}

static Ava_PLATFORM_FORCEINLINE void* MoveFwd(void* dst, const void* src, uword size)
{
	return ::memmove(dst, src, size);
}

static Ava_PLATFORM_FORCEINLINE void* MoveBwd(void* dst, const void* src, uword size)
{
	return ::memmove(dst, src, size);
}

static Ava_PLATFORM_FORCEINLINE void* Set(void* dst, byte value, uword size)
{
	return ::memset(dst, value, size);
}

static Ava_PLATFORM_FORCEINLINE void* Zero(void* dst, uword size)
{
	return ::memset(dst, 0, size);
}

static Ava_PLATFORM_FORCEINLINE i32 Compare(const void* buf1, const void* buf2, uword size)
{
	return ::memcmp(buf1, buf2, size);
}
