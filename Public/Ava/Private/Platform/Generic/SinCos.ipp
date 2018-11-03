static Ava_PLATFORM_FORCEINLINE void SinCos(f32* sin, f32* cos, f32 rad)
{
	*sin = Sin(rad);
	*cos = Cos(rad);
}

static Ava_PLATFORM_FORCEINLINE void SinCos(f64* sin, f64* cos, f64 rad)
{
	*sin = Sin(rad);
	*cos = Cos(rad);
}
