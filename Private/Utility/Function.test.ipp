TEST_SUITE_BEGIN("Utility/Function/" SUITE_NAME);

TEST_CASE("Construct")
{
	typedef FUNCTION_TYPE Function;

	SUBCASE("Default")
	{
		Function func;
		REQUIRE_UNARY_FALSE(func);
	}

	SUBCASE("Callable0")
	{
		Function func(Callable0{}, UnitTest::Allocator());
		REQUIRE_UNARY(func);
		CHECK_EQ(func(1), 1);
	}

	SUBCASE("Callable1")
	{
		Function func(Callable1{ 7 }, UnitTest::Allocator());
		REQUIRE_UNARY(func);
		CHECK_EQ(func(1), 8);
	}

	SUBCASE("Callable2")
	{
		Function func(Callable2{ 7, 42 }, UnitTest::Allocator());
		REQUIRE_UNARY(func);
		CHECK_EQ(func(1), 50);
	}
}

TEST_CASE("Copy")
{
	typedef FUNCTION_TYPE Function;

	SUBCASE("Default")
	{
		Function src;
		Function func(src);
		REQUIRE_UNARY_FALSE(func);
	}

	SUBCASE("Callable0")
	{
		Function src(Callable0{}, UnitTest::Allocator());
		Function func(src);
		REQUIRE_UNARY(func);
		CHECK_EQ(func(1), 1);
	}

	SUBCASE("Callable1")
	{
		Function src(Callable1{ 7 }, UnitTest::Allocator());
		Function func(src);
		REQUIRE_UNARY(func);
		CHECK_EQ(func(1), 8);
	}

	SUBCASE("Callable2")
	{
		Function src(Callable2{ 7, 42 }, UnitTest::Allocator());
		Function func(src);
		REQUIRE_UNARY(func);
		CHECK_EQ(func(1), 50);
	}
}

TEST_CASE("Assign")
{
	typedef FUNCTION_TYPE Function;

	SUBCASE("To empty")
	{
		Function func;

		SUBCASE("nullptr")
		{
			func = nullptr;
			REQUIRE_UNARY_FALSE(func);
		}

		SUBCASE("Callable0")
		{
			func.Assign(Callable0{}, UnitTest::Allocator());
			REQUIRE_UNARY(func);
			CHECK_EQ(func(1), 1);
		}

		SUBCASE("Callable1")
		{
			func.Assign(Callable1{ 7 }, UnitTest::Allocator());
			REQUIRE_UNARY(func);
			CHECK_EQ(func(1), 8);
		}

		SUBCASE("Callable2")
		{
			func.Assign(Callable2{ 7, 42 }, UnitTest::Allocator());
			REQUIRE_UNARY(func);
			CHECK_EQ(func(1), 50);
		}
	}

	SUBCASE("To non-empty")
	{
		Function func([](i32) { return 0; });

		SUBCASE("nullptr")
		{
			func = nullptr;
			REQUIRE_UNARY_FALSE(func);
		}

		SUBCASE("Callable0")
		{
			func.Assign(Callable0{}, UnitTest::Allocator());
			REQUIRE_UNARY(func);
			CHECK_EQ(func(1), 1);
		}

		SUBCASE("Callable1")
		{
			func.Assign(Callable1{ 7 }, UnitTest::Allocator());
			REQUIRE_UNARY(func);
			CHECK_EQ(func(1), 8);
		}

		SUBCASE("Callable2")
		{
			func.Assign(Callable2{ 7, 42 }, UnitTest::Allocator());
			REQUIRE_UNARY(func);
			CHECK_EQ(func(1), 50);
		}
	}
}

TEST_CASE("Move")
{
	typedef FUNCTION_TYPE Function;

	SUBCASE("Default")
	{
		Function src;
		Function func(Move(src));
		REQUIRE_UNARY_FALSE(func);
	}

	SUBCASE("Callable0")
	{
		Function src(Callable0{});
		Function func(Move(src));
		REQUIRE_UNARY(func);
		CHECK_EQ(func(1), 1);
	}

	SUBCASE("Callable1")
	{
		Function src(Callable1{ 7 });
		Function func(Move(src));
		REQUIRE_UNARY(func);
		CHECK_EQ(func(1), 8);
	}

	SUBCASE("Callable2")
	{
		Function src(Callable2{ 7, 42 });
		Function func(Move(src));
		REQUIRE_UNARY(func);
		CHECK_EQ(func(1), 50);
	}
}

TEST_SUITE_END();

#undef SUITE_NAME
#undef FUNCTION_TYPE
