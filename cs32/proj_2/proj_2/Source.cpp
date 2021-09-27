#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	//default constructor
	Sequence p;
	//testing an empty sequence on functions
	assert(p.empty());
	assert(p.remove(0) == 0);
	assert(p.size() == 0);
	//inserting into a sequence
	assert(p.insert(0, 0));
	assert(!p.insert(-1, 0)); //out of bounds
	assert(!p.insert(2, 0));
	assert(p.insert(2)); //insert in order
	assert(p.insert(1));
	assert(p.find(1) == 1);
	//copy constructor
	Sequence s(p);
	assert(s.find(1) == 1);
	assert(s.size() == p.size());
	//= operator
	Sequence t;
	t = s;
	assert(t.size() == s.size());
	assert(t.find(0) == 0);
	assert(t.insert(0) == 0);
	//test if remove with a single argument removes all instances
	assert(t.remove(0) == 2);
	assert(t.erase(1)); //test if erase deletes the correct position node
	assert(t.find(1) == 0);

	//test get and set functions
	Sequence birthday;
	birthday.insert(3);
	birthday.insert(23);
	birthday.insert(1998);

	ItemType val;

	birthday.get(0, val); //val should now be 3
	assert(!birthday.get(-1, val)); //val unchanged
	assert(val == 3);

	assert(!birthday.set(-1, val));
	assert(birthday.set(1, val)); //replace 23 with 3
	assert(birthday.find(23) == -1); //23 is no longer in sequence

									 //test swap function

	Sequence odds, evens, both;

	odds.insert(1);
	odds.insert(3);
	odds.insert(5);
	odds.insert(7);

	evens.insert(0);
	evens.insert(2);
	evens.insert(4);
	evens.insert(6);

	evens.swap(odds);

	assert(evens.find(1) == 0);
	assert(odds.find(0) == 0); //items should be exchanged

	odds.remove(0);
	evens.remove(1);

	odds.swap(evens); //see if swap works after sequences are modified

	assert(odds.find(1) == -1);
	assert(evens.find(0) == -1);

	//test interleave and subsequence

	interleave(evens, odds, both);

	assert(both.find(2) == 0);
	assert(both.size() == 6); //make sure sizes and values were interwoven

	Sequence subSeq;
	subSeq.insert(2);
	subSeq.insert(3);
	subSeq.insert(4);

	assert(subsequence(both, subSeq) == 0); //test subsequence
	subSeq.remove(3);

	assert(subsequence(both, subSeq) == -1); //subSeq is no longer a subsequence because it's missing 3

}

int main()
{
	test();
	cout << "Passed all tests" << endl;
}