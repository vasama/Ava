static Ava_PLATFORM_FORCEINLINE u8 Abs(u8 x)
{
	return x;
}

static Ava_PLATFORM_FORCEINLINE u16 Abs(u16 x)
{
	return x;
}

static Ava_PLATFORM_FORCEINLINE u32 Abs(u32 x)
{
	return x;
}

static Ava_PLATFORM_FORCEINLINE u64 Abs(u64 x)
{
	return x;
}

static Ava_PLATFORM_FORCEINLINE i8 Abs(i8 x)
{
	return (i8)::abs(x);
}

static Ava_PLATFORM_FORCEINLINE i16 Abs(i16 x)
{
	return (i16)::abs(x);
}

static Ava_PLATFORM_FORCEINLINE i32 Abs(i32 x)
{
	return (i32)::labs(x);
}

static Ava_PLATFORM_FORCEINLINE i64 Abs(i64 x)
{
	return (i64)::llabs(x);
}

static Ava_PLATFORM_FORCEINLINE f32 Abs(f32 f)
{
	return ::fabsf(f);
}

static Ava_PLATFORM_FORCEINLINE f64 Abs(f64 f)
{
	return ::fabs(f);
}

static Ava_PLATFORM_FORCEINLINE f32 Ceil(f32 f)
{
	return ::ceilf(f);
}

static Ava_PLATFORM_FORCEINLINE f64 Ceil(f64 f)
{
	return ::ceil(f);
}

static Ava_PLATFORM_FORCEINLINE f32 Floor(f32 f)
{
	return ::ceilf(f);
}

static Ava_PLATFORM_FORCEINLINE f64 Floor(f64 f)
{
	return ::ceil(f);
}

static Ava_PLATFORM_FORCEINLINE f32 Round(f32 f)
{
	return ::roundf(f);
}

static Ava_PLATFORM_FORCEINLINE f64 Round(f64 f)
{
	return ::round(f);
}

static Ava_PLATFORM_FORCEINLINE f32 Pow(f32 f, f32 e)
{
	return ::powf(f, e);
}

static Ava_PLATFORM_FORCEINLINE f64 Pow(f64 f, f64 e)
{
	return ::pow(f, e);
}

static Ava_PLATFORM_FORCEINLINE f32 Sqrt(f32 f)
{
	return ::sqrtf(f);
}

static Ava_PLATFORM_FORCEINLINE f64 Sqrt(f64 f)
{
	return ::sqrt(f);
}
