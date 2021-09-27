#include <ctime>

#include "Sequence.h"
#include <assert.h>
#include <iostream>

using namespace std;

const int RUNTIMES = 100;

void dumpSequence(const Sequence& s)
{
	cerr << endl << "SEQUENCE SIZE: " << s.size() << endl << "{";
	ItemType print;
	for (int i = 0; i < s.size(); i++)
	{
		s.get(i, print);
		cerr << print;
		if (i != s.size() - 1)
			cerr << ", ";
	}
	cerr << "}" << endl;
}
void emptySequence(Sequence& s)
{
	while (s.size() != 0)
		s.erase(0);
}

void test()
{
	ItemType testing;							//Need something to Sequence::get(int, ItemType&) to

	Sequence s1;
	Sequence s2(s1);							//copy constructor of empty Sequence
	s1 = s1;

	assert(s1.empty() && s2.empty());			//test that constructed Sequences are initialized empty

	assert(s1.insert("a") == 0);				//test that the following items are inserted in
	assert(s1.insert("b") == 1);				//correct positions, inserted out of alphabetical order
	assert(s1.insert("f") == 2);
	assert(s1.insert("c") == 2);
	assert(s1.insert(1, "d"));					//test that forced insertion Sequence::insert(int, const ItemType&) works
	assert(!s1.insert(6, "not inserted"));		//test that out of bounds Sequence::insert(int, const ItemType&) returns fail code

	s1 = s1;
	dumpSequence(s1);
	//OUTPUT SHOULD BE:
	//Size: 5
	//{a, d, b, c, f}

	Sequence copyTest(s1);						//test that the copy constructor works for Sequences with items
	dumpSequence(copyTest);
	//OUTPUT SHOULD BE:
	//Size: 5
	//{a, d, b, c, f}

	assert(s1.find("d") == 1);					//test that "d" is in the correct position, also tests that Sequence::find(const ItemType&)
	assert(s1.find("z") == -1);					//test that some string that wasn't even mentioned is not in the Sequence
	assert(s1.find("a") == 0);					//test that "a" is in the correct position
	assert(s1.find("b") == 2);					//test that "b" is in the correct position
	assert(s1.find("not inserted") == -1);		//test that "not inserted" was not inserted due to error bounds
	assert(s1.find("f") == s1.size() - 1);		//test that "f" is last, as it is alphabetically last

	assert(s2.insert("rocks") == 0);			//test that inserting into empty string returns 0 position
	assert(s2.insert("hills") == 0);			//test that "hills" is placed before "rocks"
	assert(s2.insert(0, "ziggurat"));			//test that forced insertion works, unalphabetical)

	dumpSequence(s2);
	//OUTPUT SHOULD BE:
	//Size: 3
	//{ziggurat, hills, rocks}

	copyTest = s2;								//test that assignment operator works for Sequences with items
	dumpSequence(copyTest);
	//Size: 3
	//{ziggurat, hills, rocks}

	s1.swap(s2);								//test that Sequence::swap(Sequence&) works
	dumpSequence(s1);
	dumpSequence(s2);
	//OUTPUT SHOULD BE:
	//Size: 3
	//{ziggurat, hills, rocks}
	//Size: 5
	//{a, d, b, c, f}

	assert(!(s1.empty() || s2.empty()));		//test that neither are empty

	assert(s2.insert(0, "f"));					//insert 4 "f" strings
	assert(s2.insert(0, "f"));
	assert(s2.insert(0, "f"));
	assert(s2.insert(0, "f"));
	assert(s2.remove("f") == 5);				//test that Sequence::remove(const ItemType&) returns # removed
	assert(s2.size() == 4);						//test that they have been removed

	dumpSequence(s2);
	//OUTPUT SHOULD BE:
	//Size: 4
	//{a, d, b, c}

	emptySequence(s2);							//empty for later test
	dumpSequence(s2);
	//OUTPUT SHOULD BE:
	//Size: 0
	//{}

	assert(s2.empty());							//confirm that s2 is empty

	s2.swap(s1);								//confirm swapping works with empty Sequences
	dumpSequence(s1);							//Sequence::swap(Sequence&) makes use of operator= and copy constructor
	dumpSequence(s2);
	//OUTPUT SHOULD BE:
	//Size: 0
	//{}
	//Size: 3
	//{ziggurat, hills, rocks}

	assert(!s1.get(0, testing) && testing == "");	//confirm that Sequence::get(int pos, ItemType&) does not work on empty Sequence
	assert(s2.set(0, "ziggyrut"));				//see if Sequence::set(int pos, const ItemType&) works
	dumpSequence(s2);
	//OUTPUT SHOULD BE:
	//Size: 3
	//{ziggyrut, hills, rocks}

	emptySequence(s2);							//series of insertions for next test of aux functions
	ItemType stuff1[10] = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" };
	ItemType stuff2[10] = { "a", "b", "c", "d", "e", "f", "g", "h", "i", "j" };
	for (int i = 0; i < 10; i++)
		assert(s1.insert(s1.size(), stuff1[i]));
	for (int i = 0; i < 10; i++)
		assert(s2.insert(s2.size(), stuff2[i]));
	dumpSequence(s1);
	dumpSequence(s2);
	//OUTPUT SHOULD BE:
	//Size: 10
	//{1, 2, 3, 4, 5, 6, 7, 8, 9, 0}
	//Size: 10
	//{a, b, c, d, e, f, g, h, i, j}

	assert(subsequence(s1, s2) == -1);			//confirm no subsequence exists, test fail case

	Sequence result;							//create new Sequence to test interleave(const Seqeunce&, const Seqeunce&, Seqeunce&)
	interleave(s1, s2, result);
	dumpSequence(result);
	//OUTPUT SHOULD BE:
	//Size: 20
	//{1, a, 2, b, 3, c, 4, d, 5, e, 6, f, 7, g, 8, h, 9, i, 0, j}

	Sequence empty;
	interleave(empty, result, result);
	dumpSequence(result);

	interleave(s2, s1, s2);						//test using same Sequences at third argument
	dumpSequence(s2);
	//OUTPUT SHOULD BE:
	//Size: 20
	//{a, 1, b, 2, c, 3, d, 4, e, 5, f, 6, g, 7, h, 8, i, 9, j, 0}

	assert(s2.erase(4));						//test that erasing specific position works
	dumpSequence(s2);
	//OUTPUT SHOULD BE:
	//Size: 19
	//{a, 1, b, 2, 3, d, 4, e, 5, f, 6, g, 7, h, 8, i, 9, j, 0}

	s2.erase(17);
	dumpSequence(s2);
	//OUTPUT SHOULD BE:
	//Size: 18
	//{a, 1, b, 2, 3, d, 4, e, 5, f, 6, g, 7, h, 8, i, 9, 0}

	s2.get(16, testing);						//thoroughly test get and set function
	assert(testing == "9");						//test that the correct value was retrieved
	s2.set(13, testing);
	s2.get(13, testing);
	assert(testing == "9");						//test that the correct value @ position was retrieved
	s2.set(4, testing);							//etc.
	s2.get(4, testing);
	s2.set(0, testing);
	s2.set(s2.size() - 1, testing);
	assert(testing == "9");

	dumpSequence(s2);							//see if set worked
												//OUTPUT SHOULD BE:
												//Size: 18									//unchanged size
												//{9, 1, b, 2, 9, d, 4, e, 5, f, 6, g, 7, 9, 8, i, 9, 9}

	emptySequence(s2);							//the following sets are reinserted into s2 to test subsequence
	ItemType stuff3[4] = { "2", "3", "4", "5", };
	for (int i = 0; i < 4; i++)
		assert(s2.insert(s2.size(), stuff3[i]));

	assert(subsequence(s1, s2) == 1);			//test that the sequence is found here

	emptySequence(s2);
	ItemType stuff4[5] = { "7", "8", "9", "0", "not at the end" };
	for (int i = 0; i < 5; i++)
		assert(s2.insert(s2.size(), stuff4[i]));

	assert(subsequence(s1, s2) == -1);			//testing to see if subsequence doesn't run off the end
}

void runTimeTest()
{
	int average = 0;
	clock_t t;
	for (int i = 0; i < RUNTIMES; i++)
	{
		t = clock();
		test();
		test();
		test();
		test();
		test();
		t = clock() - t;
		average += t;
	}
	average /= RUNTIMES;
	cerr << "RUNTIME AVERAGE: " << average << endl;
}

int main()
{
	test();
	cerr << endl << "Passed all tests." << endl;
}

//#include "Sequence.h"
//
//#define CHECKTYPE(f, t) { auto p = (t)(f); (void)p; }
//
//static_assert(std::is_default_constructible<Sequence>::value,
//	"Sequence must be default-constructible.");
//static_assert(std::is_copy_constructible<Sequence>::value,
//	"Sequence must be copy-constructible.");
//
//void thisFunctionWillNeverBeCalled()
//{
//	CHECKTYPE(&Sequence::operator=, Sequence& (Sequence::*)(const ItemType&));
//	CHECKTYPE(&Sequence::empty, bool (Sequence::*)() const);
//	CHECKTYPE(&Sequence::size, int  (Sequence::*)() const);
//	CHECKTYPE(&Sequence::insert, bool (Sequence::*)(int, const ItemType&));
//	CHECKTYPE(&Sequence::insert, int (Sequence::*)(const ItemType&));
//	CHECKTYPE(&Sequence::erase, bool (Sequence::*)(int));
//	CHECKTYPE(&Sequence::remove, int  (Sequence::*)(const ItemType&));
//	CHECKTYPE(&Sequence::get, bool (Sequence::*)(int, ItemType&) const);
//	CHECKTYPE(&Sequence::set, bool (Sequence::*)(int, const ItemType&));
//	CHECKTYPE(&Sequence::find, int  (Sequence::*)(const ItemType&) const);
//	CHECKTYPE(&Sequence::swap, void (Sequence::*)(Sequence&));
//	CHECKTYPE(subsequence, int(*)(const Sequence&, const Sequence&));
//	CHECKTYPE(interleave, void(*)(const Sequence&, const Sequence&, Sequence&));
//}
//
//int main()
//{}