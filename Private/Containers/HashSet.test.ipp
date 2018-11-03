#define TEST_PROLOGUE \
	typedef BasicHashSet<Val, Hasher, TEST_POLICY> Set;

TEST_CASE(TEST_PREFIX "/Find")
{
	TEST_PROLOGUE;

	Set set;
	REQUIRE_EQ(set.Find(Val(42)), nullptr);

	set.Insert(Val(0, 0));
	set.Insert(Val(1, 1));
	set.Insert(Val(2, 2));
	set.Insert(Val(3, 0));

	Set::ElementType* element;

	REQUIRE_UNARY(element = set.Find(Val(1, 1)));
	CHECK_EQ(element->m_value, 1);

	REQUIRE_UNARY(element = set.Find(Val(3, 0)));
	CHECK_EQ(element->m_value, 3);
}

TEST_CASE(TEST_PREFIX "/Insert")
{
	TEST_PROLOGUE;

	Set set;
	CHECK_UNARY(set.Insert(Val(0, 0)).Inserted);
	CHECK_UNARY(set.Insert(Val(1, 1)).Inserted);
	CHECK_UNARY(set.Insert(Val(2, 2)).Inserted);
	CHECK_UNARY(set.Insert(Val(3, 3)).Inserted);
	CHECK_UNARY(set.Insert(Val(4, 0)).Inserted);
	CHECK_UNARY(set.Insert(Val(5, 1)).Inserted);
	CHECK_UNARY(set.Insert(Val(6, 2)).Inserted);
	CHECK_UNARY(set.Insert(Val(7, 0)).Inserted);

	REQUIRE_EQ(set.Size(), 8);
}

TEST_CASE(TEST_PREFIX "/Remove")
{
	TEST_PROLOGUE;

	Set set;
	CHECK_UNARY_FALSE(set.Remove(Val(0)));

	set.Insert(Val(0, 0));
	set.Insert(Val(1, 1));
	set.Insert(Val(2, 2));
	set.Insert(Val(3, 0));

	CHECK_UNARY(set.Remove(Val(1, 1)));
	CHECK_UNARY(set.Remove(Val(3, 0)));

	REQUIRE_EQ(set.Size(), 2);
}

#ifdef TEST_REHASH
TEST_CASE(TEST_PREFIX "/Rehash")
{
	TEST_PROLOGUE;

	Set set;
	set.Reserve(1);

	iword cap = set.Capacity();
	for (i32 i = 0; i < cap + 1; ++i)
		set.Insert(Val(i));

	CHECK_EQ(set.Size(), cap + 1);
	CHECK_GT(set.Capacity(), cap);
}
#	undef TEST_REHASH
#endif

#undef TEST_PREFIX
#undef TEST_POLICY
