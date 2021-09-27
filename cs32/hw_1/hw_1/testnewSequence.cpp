#include "newSequence.h"
#include <iostream>
#include <cassert>
using namespace std;

// To test a Sequence of unsigned long, leave the following #define line
// commented out; to test a Sequence of string, remove the "//".

// #define TEST_WITH_STRING

#ifdef TEST_WITH_STRING
const ItemType VAL[3] = { "injera", "lavash", "tortilla" };
#else // assume unsigned long
const ItemType VAL[3] = { 999, 10, 20 };
#endif

void test()
{
	Sequence a(1000);   // a can hold at most 1000 items
	Sequence b(5);      // b can hold at most 5 items
	Sequence c;         // c can hold at most DEFAULT_MAX_ITEMS items
	ItemType v = 3;

	// No failures inserting 5 items into b
	for (int k = 0; k < 5; k++)
		assert(b.insert(k, v));

	// Failure if we try to insert a sixth item into b
	assert(!b.insert(5, v));

	// When two Sequences' contents are swapped, their capacities are
	// swapped as well:
	a.swap(b);
	assert(!a.insert(5, v));
	for (int k = 0; k < 1000; k++)
		assert(b.insert(k, v));

	//setup
	Sequence s1(2);					//should be: {VAL[1], VAL[0]}
	s1.insert(0, VAL[0]);
	s1.insert(0, VAL[1]);
	Sequence s2(1);					//should be: {VAL[2]}
	s2.insert(0, VAL[2]);
	s1.swap(s2);					//swap

	//each test separated
	assert(s1.size() == 1);			//ensure that the size has been properly assigned to s1
	assert(s1.find(VAL[2]) == 0);		//make sure that the unsigned long is in the right position
	assert(s2.size() == 2);			//ensure that the size has been properly assigned to s2
	assert(s2.find(VAL[1]) == 0);		//this and the next line ensure that the long elements have
	assert(s2.find(VAL[0]) == 1);		//been assigned in the correct positions

	//confirm that you can't add more elements to the full Sequence
	assert(!s1.insert(1, v));
	assert(!s2.insert(2, v));
}

int main()
{
	test();
	cout << "Passed all tests." << endl;
}