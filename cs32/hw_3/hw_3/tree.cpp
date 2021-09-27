/*
HOMEWORK 3
Christian Yu
104785497
CS32-2 w/ Smallberg
*/

// Return the number of ways that all n2 elements of a2 appear
// in the n1 element array a1 in the same order (though not
// necessarily consecutively).  The empty sequence appears in a
// sequence of length n1 in 1 way, even if n1 is 0.
// For example, if a1 is the 7 element array
//	10 50 40 20 50 40 30
// then for this value of a2     the function must return
//	10 20 40			1
//	10 40 30			2
//	20 10 40			0
//	50 40 30			3
int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
	if (n2 <= 0)								//base case 1, finished searching a2
		return 1;
	if (n1 <= 0 && n2 > 0)						//base case 2, ended without finishing a2
		return 0;
	int check = 0;
	if (a1[0] == a2[0])							//matches, new search
		check += countIncludes(a1 + 1, n1 - 1, a2 + 1, n2 - 1);
	return check + countIncludes(a1 + 1, n1 - 1, a2, n2);	//continue current search
}

// Exchange two doubles
void exchange(double& x, double& y)
{
	double t = x;
	x = y;
	y = t;
}

// Rearrange the elements of the array so that all the elements
// whose value is > separator come before all the other elements,
// and all the elements whose value is < separator come after all
// the other elements.  Upon return, firstNotGreater is set to the
// index of the first element in the rearranged array that is
// <= separator, or n if there is no such element, and firstLess is
// set to the index of the first element that is < separator, or n
// if there is no such element.
// In other words, upon return from the function, the array is a
// permutation of its original value such that
//   * for 0 <= i < firstNotGreater, a[i] > separator
//   * for firstNotGreater <= i < firstLess, a[i] == separator
//   * for firstLess <= i < n, a[i] < separator
// All the elements > separator end up in no particular order.
// All the elements < separator end up in no particular order.
void separate(double a[], int n, double separator,
	int& firstNotGreater, int& firstLess)
{
	if (n < 0)
		n = 0;

	// It will always be the case that just before evaluating the loop
	// condition:
	//  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
	//  Every element earlier than position firstNotGreater is > separator
	//  Every element from position firstNotGreater to firstUnknown-1 is
	//    == separator
	//  Every element from firstUnknown to firstLess-1 is not known yet
	//  Every element at position firstLess or later is < separator

	firstNotGreater = 0;
	firstLess = n;
	int firstUnknown = 0;
	while (firstUnknown < firstLess)
	{
		if (a[firstUnknown] < separator)
		{
			firstLess--;
			exchange(a[firstUnknown], a[firstLess]);
		}
		else
		{
			if (a[firstUnknown] > separator)
			{
				exchange(a[firstNotGreater], a[firstUnknown]);
				firstNotGreater++;
			}
			firstUnknown++;
		}
	}
}

// Rearrange the elements of the array so that
// a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
// If n <= 1, do nothing.
void order(double a[], int n)
{
	if (n > 1)
	{
		int firstNotGreater, firstLess;					//need int for separate
		separate(a, n, a[0], firstNotGreater, firstLess);	//separate the sequence using the first index
		order(a, firstNotGreater);						//order the first half
		order(a + firstLess, n - firstLess);			//order the second half, operates in a binary fashion
	}
}

#include <iostream>
#include <assert.h>
using namespace std;

int main()
{
	double things[10] = { 0.3, 42, 33, -3.2, -0.3, 0.3, 46, 999, -999, 1 };
	order(things, 10);
	for (int i = 0; i < 10; i++)
	{
		cout << *(things + i);
		if (i != 9)
			cout << ", ";
	}
	cout << endl;
	
	double bigOne[10] = { 1, 2, 3, 2, 3, 6, 7, 8, 8, 10 };
	double incOne[3] = { 1, 3, 8 };
	double incTwo[4] = { 2,3,7,8 };
	double notInc[5] = { 1,2,3,2,1 };
	cout << countIncludes(bigOne, 10, incOne, 3) << endl;
	cout << countIncludes(bigOne, 10, incTwo, 4) << endl;
	cout << countIncludes(bigOne, 10, notInc, 5) << endl;
}
