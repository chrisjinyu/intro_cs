#include "ScoreList.h"

#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	ScoreList l1;

	//test initial conditions, is empty
	assert(l1.minimum() == NO_SCORE);
	assert(l1.maximum() == NO_SCORE);
	assert(l1.size() == 0);

	//test adding invalid scores
	assert(!l1.add(-3));
	assert(!l1.add(105));

	//test that nothing has been added yet
	assert(l1.size() == 0);

	//add valid score and assess that it has been added properly
	assert(l1.add(100));
	assert(l1.size() == 1);
	assert(l1.maximum() == 100 && l1.minimum() == 100);

	//remove score and assess that it has been removed properly
	assert(l1.remove(100));
	assert(l1.size() == 0);
	assert(l1.maximum() == NO_SCORE && l1.minimum() == NO_SCORE);

	//add in a set of DEFAULT_MAX_ITEMS scores, complete fill
	for (int i = 0; i < DEFAULT_MAX_ITEMS; i++)
	{
		if (i <= 100)
			assert(l1.add((unsigned long)i));
		else
			assert(l1.add((unsigned long)(i - 101)));
	}

	//confirm that it is full and no more can be added
	assert(l1.size() == DEFAULT_MAX_ITEMS);
	assert(!l1.add(50));

	//confirm that the min and max are as added
	assert(l1.minimum() == 0);
	assert(l1.maximum() == 100);
}

int main()
{
	test();
	cout << "Passed all tests." << endl;
}