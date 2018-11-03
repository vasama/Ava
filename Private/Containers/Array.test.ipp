TEST_CASE(TEST_PREFIX "/Copy")
{
	TEST_PROLOGUE;

	Array src;
	src.Append(Val(1));
	src.Append(Val(2));
	src.Append(Val(3));

	Array array(src);
	REQUIRE_EQ(array.Size(), 3);
	CHECK_EQ(array[0].m_value, 1);
	CHECK_EQ(array[1].m_value, 2);
	CHECK_EQ(array[2].m_value, 3);
}

TEST_CASE(TEST_PREFIX "/Move")
{
	TEST_PROLOGUE;

	Array src;
	src.Append(Val(1));
	src.Append(Val(2));
	src.Append(Val(3));

	Array array(Move(src));
	REQUIRE_EQ(array.Size(), 3);
	CHECK_EQ(array[0].m_value, 1);
	CHECK_EQ(array[1].m_value, 2);
	CHECK_EQ(array[2].m_value, 3);
}

TEST_CASE(TEST_PREFIX "/Append")
{
	TEST_PROLOGUE;

	Array array;

	array.Append(Val(1));
	REQUIRE_EQ(array.Size(), 1);
	CHECK_EQ(array[0].m_value, 1);

	array.Append(Val(2));
	REQUIRE_EQ(array.Size(), 2);
	CHECK_EQ(array[0].m_value, 1);
	CHECK_EQ(array[1].m_value, 2);
}

TEST_CASE(TEST_PREFIX "/AppendRange")
{
	TEST_PROLOGUE;

	Array array;

	{
		Val input[1] = { Val(1) };
		array.AppendRange(Span(input));
	}
	REQUIRE_EQ(array.Size(), 1);
	CHECK_EQ(array[0].m_value, 1);

	{
		Val input[2] = { Val(2), Val(3) };
		array.AppendRange(Span(input));
	}
	REQUIRE_EQ(array.Size(), 3);
	CHECK_EQ(array[0].m_value, 1);
	CHECK_EQ(array[1].m_value, 2);
	CHECK_EQ(array[2].m_value, 3);
}

TEST_CASE(TEST_PREFIX "/Insert")
{
	TEST_PROLOGUE;

	Array array;

	array.Insert(0, Val(1));
	REQUIRE_EQ(array.Size(), 1);
	CHECK_EQ(array[0].m_value, 1);

	array.Insert(0, Val(2));
	REQUIRE_EQ(array.Size(), 2);
	CHECK_EQ(array[0].m_value, 2);
	CHECK_EQ(array[1].m_value, 1);

	array.Insert(2, Val(3));
	REQUIRE_EQ(array.Size(), 3);
	CHECK_EQ(array[0].m_value, 2);
	CHECK_EQ(array[1].m_value, 1);
	CHECK_EQ(array[2].m_value, 3);
}

TEST_CASE(TEST_PREFIX "/InsertRange")
{
	TEST_PROLOGUE;

	Array array;

	{
		Val input[1] = { Val(1) };
		array.InsertRange(0, Span(input));
	}
	REQUIRE_EQ(array.Size(), 1);
	CHECK_EQ(array[0].m_value, 1);

	{
		Val input[2] = { { 2 },{ 3 } };
		array.InsertRange(0, Span(input));
	}
	REQUIRE_EQ(array.Size(), 3);
	CHECK_EQ(array[0].m_value, 2);
	CHECK_EQ(array[1].m_value, 3);
	CHECK_EQ(array[2].m_value, 1);

	{
		Val input[1] = { { 4 } };
		array.InsertRange(3, Span(input));
	}
	REQUIRE_EQ(array.Size(), 4);
	CHECK_EQ(array[0].m_value, 2);
	CHECK_EQ(array[1].m_value, 3);
	CHECK_EQ(array[2].m_value, 1);
	CHECK_EQ(array[3].m_value, 4);

	{
		Val input[2] = { { 5 },{ 6 } };
		array.InsertRange(4, Span(input));
	}
	REQUIRE_EQ(array.Size(), 6);
	CHECK_EQ(array[0].m_value, 2);
	CHECK_EQ(array[1].m_value, 3);
	CHECK_EQ(array[2].m_value, 1);
	CHECK_EQ(array[3].m_value, 4);
	CHECK_EQ(array[4].m_value, 5);
	CHECK_EQ(array[5].m_value, 6);
}

TEST_CASE(TEST_PREFIX "/Remove")
{
	TEST_PROLOGUE;

	Array array;
	array.Append(Val(1));
	array.Append(Val(2));
	array.Append(Val(3));
	array.Append(Val(4));

	array.Remove(0);
	REQUIRE_EQ(array.Size(), 3);
	CHECK_EQ(array[0].m_value, 2);
	CHECK_EQ(array[1].m_value, 3);
	CHECK_EQ(array[2].m_value, 4);

	array.Remove(1);
	REQUIRE_EQ(array.Size(), 2);
	CHECK_EQ(array[0].m_value, 2);
	CHECK_EQ(array[1].m_value, 4);

	array.Remove(1);
	REQUIRE_EQ(array.Size(), 1);
	CHECK_EQ(array[0].m_value, 2);

	array.Remove(0);
	REQUIRE_EQ(array.Size(), 0);
}

TEST_CASE(TEST_PREFIX "/RemoveRange")
{
	TEST_PROLOGUE;

	Array array;
	array.Append(Val(1));
	array.Append(Val(2));
	array.Append(Val(3));
	array.Append(Val(4));
	array.Append(Val(5));
	array.Append(Val(6));
	array.Append(Val(7));
	array.Append(Val(8));

	array.RemoveRange(0, 2);
	REQUIRE_EQ(array.Size(), 6);
	CHECK_EQ(array[0].m_value, 3);
	CHECK_EQ(array[1].m_value, 4);
	CHECK_EQ(array[2].m_value, 5);
	CHECK_EQ(array[3].m_value, 6);
	CHECK_EQ(array[4].m_value, 7);
	CHECK_EQ(array[5].m_value, 8);

	array.RemoveRange(1, 2);
	REQUIRE_EQ(array.Size(), 4);
	CHECK_EQ(array[0].m_value, 3);
	CHECK_EQ(array[1].m_value, 6);
	CHECK_EQ(array[2].m_value, 7);
	CHECK_EQ(array[3].m_value, 8);

	array.RemoveRange(2, 2);
	REQUIRE_EQ(array.Size(), 2);
	CHECK_EQ(array[0].m_value, 3);
	CHECK_EQ(array[1].m_value, 6);

	array.RemoveRange(0, 2);
	REQUIRE_EQ(array.Size(), 0);
}

TEST_CASE(TEST_PREFIX "/Extract")
{
	TEST_PROLOGUE;

	Array array;
	array.Append(Val(1));
	array.Append(Val(2));
	array.Append(Val(3));

	{
		Val value = array.Extract(0);
		CHECK_EQ(value.m_value, 1);
	}
	REQUIRE_EQ(array.Size(), 2);
	CHECK_EQ(array[0].m_value, 2);
	CHECK_EQ(array[1].m_value, 3);

	{
		Val value = array.Extract(1);
		CHECK_EQ(value.m_value, 3);
	}
	REQUIRE_EQ(array.Size(), 1);
	CHECK_EQ(array[0].m_value, 2);

	{
		Val value = array.Extract(0);
		CHECK_EQ(value.m_value, 2);
	}
	REQUIRE_EQ(array.Size(), 0);
}

TEST_CASE(TEST_PREFIX "/Resize")
{
	TEST_PROLOGUE;

	Array array;

	array.Resize(1);
	REQUIRE(array.Size() == 1);

	array.Resize(10);
	REQUIRE(array.Size() == 10);

	array.Resize(9);
	REQUIRE(array.Size() == 9);

	array.Resize(0);
	REQUIRE(array.Size() == 0);
}

TEST_CASE(TEST_PREFIX "/Clear")
{
	TEST_PROLOGUE;

	Array array;

	array.Clear();
	CHECK(array.Size() == 0);

	array.Append(Val(0));

	array.Clear();
	CHECK(array.Size() == 0);
}

#undef TEST_PROLOGUE
#undef TEST_PREFIX
