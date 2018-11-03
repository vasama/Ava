static Ava_PLATFORM_FORCEINLINE u8 Rotl(u8 x, u8 shift)
{
	return (u8)((x << shift) | (x >> (8 - shift)));
}

static Ava_PLATFORM_FORCEINLINE u16 Rotl(u16 x, u16 shift)
{
	return (u16)((x << shift) | (x >> (16 - shift)));
}

static Ava_PLATFORM_FORCEINLINE u32 Rotl(u32 x, u32 shift)
{
	return (u32)((x << shift) | (x >> (32 - shift)));
}

static Ava_PLATFORM_FORCEINLINE u64 Rotl(u64 x, u64 shift)
{
	return (u64)((x << shift) | (x >> (64 - shift)));
}

static Ava_PLATFORM_FORCEINLINE u8 Rotr(u8 x, u8 shift)
{
	return (u8)((x >> shift) | (x << (8 - shift)));
}

static Ava_PLATFORM_FORCEINLINE u16 Rotr(u16 x, u16 shift)
{
	return (u16)((x >> shift) | (x << (16 - shift)));
}

static Ava_PLATFORM_FORCEINLINE u32 Rotr(u32 x, u32 shift)
{
	return (u32)((x >> shift) | (x << (32 - shift)));
}

static Ava_PLATFORM_FORCEINLINE u64 Rotr(u64 x, u64 shift)
{
	return (u64)((x >> shift) | (x << (64 - shift)));
}
