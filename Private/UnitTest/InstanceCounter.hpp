#pragma once

namespace Ava::UnitTest {

class InstanceCounter
{
protected:
	InstanceCounter()
	{
		Increment();
	}

	InstanceCounter(InstanceCounter&&)
	{
		Increment();
	}

	InstanceCounter(const InstanceCounter&)
	{
		Increment();
	}

	~InstanceCounter()
	{
		Decrement();
	}

private:
	static void Increment();
	static void Decrement();
};

} // namespace Ava::UnitTest
