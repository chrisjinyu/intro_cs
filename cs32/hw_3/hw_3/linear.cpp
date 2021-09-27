#include <assert.h>
#include <iostream>

using namespace std;

bool somePredicate(double x)
{
	return x < 0;
}

/*
HOMEWORK 3
Christian Yu
104785497
CS32-2 w/ Smallberg
*/

// Return false if the somePredicate function returns false for at
// least one of the array elements; return true otherwise.
bool allTrue(const double a[], int n)
{
	if (n <= 0)							//base case, end of array
		return true;
	if (!somePredicate(a[0]))			//base case, found false
		return false;
	return allTrue(a + 1, n - 1);		//continue, implied &&
}

// Return the number of elements in the array for which the
// somePredicate function returns false.
int countFalse(const double a[], int n)
{
	if (n <= 0)									//base case, end of array
		return 0;
	if (somePredicate(a[0]))					//not false, continue search
		return countFalse(a + 1, n - 1);
	return 1 + countFalse(a + 1, n - 1);		//add 1 if false, continue search
}

// Return the subscript of the first element in the array for which
// the somePredicate function returns false.  If there is no such
// element, return -1.
int firstFalse(const double a[], int n)
{
	if (n <= 0)									//base case, end of array, -1
		return -1;
	if (!somePredicate(a[0]))					//base case, false found
		return 0;
	int check = firstFalse(a + 1, n - 1);		//initialize checking integer, searches deeper
	if (check == -1)							//end of array w/o finding false, return -1
		return -1;
	return 1 + check;							//pops out, adds 1 for each index
}

// Return the subscript of the smallest double in the array (i.e.,
// the one whose value is <= the value of all elements).  If more
// than one element has the same smallest value, return the smallest
// subscript of such an element.  If the array has no elements to
// examine, return -1.
int indexOfMin(const double a[], int n)
{
	if (n == 1)									//base case, one index
		return 0;
	if (n <= 0)									//base case, no searchable items
		return -1;
	int pos = 1 + indexOfMin(a + 1, n - 1);		//continues until reaches base, 0 or -1
	if (a[0] > a[pos])							//check for minimum
		return pos;								//pops out by returning the minimum
	else
		return 0;								//remains
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0)								//base case, finished searching a2, is included
		return true;
	if (n1 <= 0 && n2 > 0)						//base case, ended without finishing a2, is not included
		return false;
	if (a1[0] == a2[0])							//match found, continue searching here
		return includes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	return includes(a1 + 1, n1 - 1, a2, n2);	//continue searching
}

int main()
{
	double things[5] = { -0.1, -2, 3, -4, 5 };
	assert(!allTrue(things, 5));
	assert(countFalse(things, 5) == 2);
	cout << firstFalse(things, 5) << endl;
	double others[5] = { -0.1, -2, -3, -4, -5 };
	cout << firstFalse(others, 5) << endl;
	assert(countFalse(others, 5) == 0);
	cout << indexOfMin(things, 5) << endl;
	double single[7] = { 4, 4, 4, -4, -4, 5, -4 };
	cout << indexOfMin(single, 7) << endl;
	assert(!includes(things, 5, others, 5));
	double included[3] = { -0.1, 3, 5 };
	assert(includes(things, 5, included, 3));
}