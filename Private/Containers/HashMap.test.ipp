#define TEST_PROLOGUE \
	typedef BasicHashMap<Key, Val, Hasher, TEST_POLICY> Map;

TEST_CASE(TEST_PREFIX "/Find")
{
	TEST_PROLOGUE;

	Map map;
	REQUIRE_EQ(map.Find(Key(42)), nullptr);

	map.Insert(Key(0, 0), Val(0));
	map.Insert(Key(1, 1), Val(1));
	map.Insert(Key(2, 2), Val(2));
	map.Insert(Key(3, 0), Val(3));

	Map::ElementType* element;

	REQUIRE_UNARY(element = map.Find(Key(1, 1)));
	CHECK_EQ(element->Value.m_value, 1);

	REQUIRE_UNARY(element = map.Find(Key(3, 0)));
	CHECK_EQ(element->Value.m_value, 3);
}

TEST_CASE(TEST_PREFIX "/Insert")
{
	TEST_PROLOGUE;

	Map map;
	CHECK_UNARY(map.Insert(Key(0, 0), Val(1)).Inserted);
	CHECK_UNARY(map.Insert(Key(1, 1), Val(2)).Inserted);
	CHECK_UNARY(map.Insert(Key(2, 2), Val(3)).Inserted);
	CHECK_UNARY(map.Insert(Key(3, 3), Val(4)).Inserted);
	CHECK_UNARY(map.Insert(Key(4, 0), Val(5)).Inserted);
	CHECK_UNARY(map.Insert(Key(5, 1), Val(6)).Inserted);
	CHECK_UNARY(map.Insert(Key(6, 2), Val(7)).Inserted);
	CHECK_UNARY(map.Insert(Key(7, 0), Val(8)).Inserted);

	REQUIRE_EQ(map.Size(), 8);
}

TEST_CASE(TEST_PREFIX "/Remove")
{
	TEST_PROLOGUE;

	Map map;
	CHECK_UNARY_FALSE(map.Remove(Key(0)));

	map.Insert(Key(0, 0), Val(0));
	map.Insert(Key(1, 1), Val(1));
	map.Insert(Key(2, 2), Val(2));
	map.Insert(Key(3, 0), Val(3));

	CHECK_UNARY(map.Remove(Key(1, 1)));
	CHECK_UNARY(map.Remove(Key(3, 0)));

	REQUIRE_EQ(map.Size(), 2);
}

#ifdef TEST_REHASH
TEST_CASE(TEST_PREFIX "/Rehash")
{
	TEST_PROLOGUE;

	Map map;
	map.Reserve(1);

	iword cap = map.Capacity();
	for (i32 i = 0; i < cap + 1; ++i)
		map.Insert(Key(i), Val(i));

	CHECK_EQ(map.Size(), cap + 1);
	CHECK_GT(map.Capacity(), cap);
}
#	undef TEST_REHASH
#endif

#undef TEST_PREFIX
#undef TEST_POLICY
