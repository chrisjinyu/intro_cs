/*
cs31 smallberg
project 4
104785497
*/
#include <iostream>				//used only for testing purposes
#include <string>
#include <cctype>
#include <cassert>				//for testing
using namespace std;

//Append value to the end of each of the n elements of the array and return n.
int appendToAll(string a[], int n, string value) {
	if (n < 0)										//invalid parameter case
		return -1;
	for (int i = 0; i < n; i++)						//for each string, adds value to string
		a[i] += value;
	return n;										//return size
}

//Return the position of a string in the array that is equal to target; 
//if there is more than one such string, return the smallest position number of such a matching string. 
//Return −1 if there is no such string.
int lookup(const string a[], int n, string target) {
	if (n < 0)										//invalid parameter case
		return -1;
	for (int i = 0; i < n;i++) {					//for each string, find the string target
		if (a[i] == target)
			return i;								//found string, return index
	}
	return -1;										//not found, return -1
}

//Return the position of a string in the array such that that string is >= every string in the array.
//If there is more than one such string, return the smallest position in the array of such a string.
//Return −1 if the array has no elements.
int positionOfMax(const string a[], int n) {
	if (n <= 0)										//invalid parameter
		return -1;

	int maxPos = 0;
	string max = a[0];								//sets maximum index and maximum to the first

	for (int i = 1; i < n; i++) {					//for each after the first, set to the current IF higher
		if (a[i] > max) {
			maxPos = i;
			max = a[i];
		}
	}
	return maxPos;									//return max
}

//Eliminate the item at position pos by copying all elements after it one place to the left.
//Put the item that was thus eliminated into the last position of the array.
//Return the original position of the item that was moved to the end.
int rotateLeft(string a[], int n, int pos) {
	if (n < 0 || pos >= n || pos < 0)				//invalid parameter
		return -1;
	string position = a[pos];						//saves to temporary string
	for (int i = pos + 1; i < n; i++)				//shifts strings from position
		a[i - 1] = a[i];
	a[n - 1] = position;							//sets final string to temporary
	return pos;										
}

//For future use in separate()
int rotateRight(string a[], int n, int pos) {
	if (n < 0 || pos >= n || pos < 0)
		return -1;
	string position = a[pos];
	for (int i = pos - 1; i >= 0; i--)
		a[i + 1] = a[i];
	a[0] = position;
	return pos;
}

//Return the number of sequences of one or more consecutive identical items in a.
int countRuns(const string a[], int n) {
	if (n < 0)										//invalid parameters
		return -1;
	if (n == 0)										//empty case
		return 0;
	int count = 1;
	for (int i = 1; i < n; i++) {					//finds number of times runs change
		if (a[i] != a[i - 1])
			count++;
	}
	return count;
}

//Reverse the order of the elements of the array and return n.
int flip(string a[], int n) {
	if (n < 0)										//invalid parameters
		return -1;
	string temp;									//temp used to swap
	for (int i = 0; i < n / 2; i++) {				//only needs half the array
		temp = a[i];
		a[i] = a[n - i - 1];
		a[n - i - 1] = temp;						//classic swap: save to temp, switch, switch to temp
	}
	return n;
}

int maximum(int a, int b) {
	if (a >= b)
		return a;
	else
		return b;
}

int minimum(int a, int b) {
	if (a <= b)
		return a;
	else
		return b;
}

//Return the position of the first corresponding elements of a1 and a2 that are not equal.
//n1 is the number of interesting elements in a1, and n2 is the number of interesting elements in a2.
//If the arrays are equal up to the point where one or both runs out, return whichever value of n1 and n2 is less than or equal to the other.
int differ(const string a1[], int n1, const string a2[], int n2) {
	if (n1 < 0 || n2 < 0)							//invalid parameters
		return -1;
	int max = maximum(n1, n2), min = minimum(n1, n2);		//max and min
	for (int i = 0; i < max; i++) {
		if (i == min)								//if reaches the lower bound, returns lower
			return min;
		if (a1[i] != a2[i])							//finds first difference
			return i;
	}
	return max;										//returns equal
}

//If all n2 elements of a2 appear in a1, consecutively and in the same order, then return the position in a1 where that subsequence begins.
//If the subsequence appears more than once in a1, return the smallest such beginning position in the array.
//Return −1 if a1 does not contain a2 as a contiguous subsequence.
int subsequence(const string a1[], int n1, const string a2[], int n2) {
	if (n1 < 0 || n2 < 0 || n2>n1)					//invalid parameters
		return -1;
	if (n2 == 0)
		return 0;
	for (int i = 0; i <= n1 - n2; i++) {			//bound set to n1-n2 because that is the most possible space required to contain
		if (a1[i] == a2[0]) {
			int count = 1;							//counts if sequence matching is correct
			for (int j = 1; j < n2; j++) {
				if (a1[i + j] == a2[j])
					count++;
				else
					j = n2;							//exit forloop
			}
			if (count == n2)
				return i;
		}
	}
	return -1;
}

//Return the smallest position in a1 of an element that is equal to any of the elements in a2. 
//Return −1 if no element of a1 is equal to any element of a2.
int lookupAny(const string a1[], int n1, const string a2[], int n2) {
	if (n1 < 0 || n2 < 0)
		return -1;
	for (int i = 0; i < n1; i++) {					//nested forloop search
		for (int j = 0; j < n2; j++)				//searches a2 for every a1 element
			if (a1[i] == a2[j])
				return i;
	}
	return -1;
}

//Rearrange the elements of the array so that all the elements whose value is < separator come before all the other elements,
//and all the elements whose value is > separator come after all the other elements.Return the position of the first element that,
//after the rearrangement, is not < separator, or n if there are no such elements.
int separate(string a[], int n, string separator) {
	if (n < 0)
		return -1;

	int index = 0;									//left pivot sort, uses index to keep track of which element is being distributed
	for (int i = 0; i < n; i++) {
		if (a[index] > separator)
			rotateLeft(a, n, index);				//sends element to far right if greater
		else if (a[index] < separator) {
			rotateRight(a, n, index);				//sends element to far left if smaller
			index++;								//has to increment index in order to not check the same element twice
		}
		else
			index++;								//see above
	}

	for (int i = 0; i < n; i++) {
		if (a[i] >= separator)
			return i;								//finds position; index+1 cannot be used since equals are also counted
	}
	return n;
}


//DEBUGGING ONLY
int main() {
	string h[7] = { "jill", "hillary", "donald", "tim", "", "evan", "gary" };
	assert(lookup(h, 7, "evan") == 5);
	assert(lookup(h, 7, "donald") == 2);
	assert(lookup(h, 2, "donald") == -1);
	assert(positionOfMax(h, 7) == 3);

	string g[4] = { "jill", "hillary", "gary", "mindy" };
	assert(differ(h, 4, g, 4) == 2);
	assert(appendToAll(g, 4, "?") == 4 && g[0] == "jill?" && g[3] == "mindy?");
	assert(rotateLeft(g, 4, 1) == 1 && g[1] == "gary?" && g[3] == "hillary?");

	string e[4] = { "donald", "tim", "", "evan" };
	assert(subsequence(h, 7, e, 4) == 2);

	string d[5] = { "hillary", "hillary", "hillary", "tim", "tim" };
	assert(countRuns(d, 5) == 2);

	string f[3] = { "gary", "donald", "mike" };
	assert(lookupAny(h, 7, f, 3) == 2);
	assert(flip(f, 3) == 3 && f[0] == "mike" && f[2] == "gary");

	assert(separate(h, 7, "gary") == 3);

	cout << "All preliminary tests succeeded." << endl << endl << endl;

	string test[128];
	int counter = 0;
	int size = 128;
	int numTest = 1;

	while (counter < 128) {
		cout << "Enter a string to go into the array:  ";
		getline(cin, test[counter]);
		if (test[counter] == "qqq") {
			size = counter;
			counter = 128;
		}
		else
			counter++;
	}

	cout << "Your array is: {";
	for (int i = 0; i < size - 1; i++)
		cout << "\"" << test[i] << "\"" << ", ";
	cout << "\"" << test[size - 1] << "\"" << "}" << endl;
	cout << "It is " << size << " elements long." << endl << endl;

	while (numTest >= 0) {
		cout << "Choose a function to test:" << endl;
		cout << "0: appendToAll" << endl;
		cout << "1: lookup" << endl;
		cout << "2: positionOfMax" << endl;
		cout << "3: rotateLeft" << endl;
		cout << "4: countRuns" << endl;
		cout << "5: flip" << endl;
		cout << "6: differ" << endl;
		cout << "7: subsequence" << endl;
		cout << "8: lookupAny" << endl;
		cout << "9: separate" << endl;
		cout << "10: Redo your string inputs" << endl << endl;

		cin >> numTest;
		cin.ignore(10000, '\n');
		if (numTest < 0)
			return 0;

		if (numTest == 10)
			main();

		int nSize = 129;
		while (nSize > size) {
			cout << "Give a valid size: ";
			cin >> nSize;
			cin.ignore(10000, '\n');
		}

		string append, look, separator;
		int position;

		if (numTest == 0) {
			cout << "Enter appending string: ";
			getline(cin, append);
			cout << "Return code: " << appendToAll(test, nSize, append) << endl;
			cout << "Your new array: {";
			for (int i = 0; i < size - 1; i++)
				cout << "\"" << test[i] << "\"" << ", ";
			cout << "\"" << test[size - 1] << "\"" << "}" << endl;
			cout << "It is " << size << " elements long." << endl << endl;
		}
		else if (numTest == 1) {
			cout << "Enter lookup string: ";
			getline(cin, look);
			cout << "Return code: " << lookup(test, nSize, look) << endl << endl;
		}
		else if (numTest == 2) {
			cout << "Return code: " << positionOfMax(test, nSize) << endl << endl;
		}
		else if (numTest == 3) {
			cout << "Enter position: ";
			cin >> position;
			cin.ignore(10000, '\n');
			cout << "Return code: " << rotateLeft(test, nSize, position) << endl;
			cout << "Your new array: {";
			for (int i = 0; i < size - 1; i++)
				cout << "\"" << test[i] << "\"" << ", ";
			cout << "\"" << test[size - 1] << "\"" << "}" << endl;
			cout << "It is " << size << " elements long." << endl << endl;
		}
		else if (numTest == 4) {
			cout << "Return code: " << countRuns(test, nSize) << endl << endl;
		}
		else if (numTest == 5) {
			cout << "Return code: " << flip(test, nSize) << endl;
			cout << "Your new array: {";
			for (int i = 0; i < size - 1; i++)
				cout << "\"" << test[i] << "\"" << ", ";
			cout << "\"" << test[size - 1] << "\"" << "}" << endl;
			cout << "It is " << size << " elements long." << endl << endl;
		}
		else if (numTest == 9) {
			cout << "Enter a separator: ";
			getline(cin, separator);
			cout << "Return code: " << separate(test, nSize, separator) << endl;
			cout << "Your new array: {";
			for (int i = 0; i < size - 1; i++)
				cout << "\"" << test[i] << "\"" << ", ";
			cout << "\"" << test[size - 1] << "\"" << "}" << endl;
			cout << "It is " << size << " elements long." << endl << endl;
		}
		else {

			string second[128];
			int sncounter = 0;
			int snsize = 128;

			while (sncounter < 128) {
				cout << "Enter a string to go into the second array:  ";
				getline(cin, second[sncounter]);
				if (second[sncounter] == "qqq") {
					snsize = sncounter;
					sncounter = 128;
				}
				else
					sncounter++;
			}

			cout << "Your second array is: {";
			for (int i = 0; i < snsize - 1; i++)
				cout << "\"" << second[i] << "\"" << ", ";
			cout << "\"" << second[snsize - 1] << "\"" << "}" << endl;
			cout << "It is " << snsize << " elements long." << endl << endl;

			int snSize = 129;
			while (snSize > snsize) {
				cout << "Give a valid size: ";
				cin >> snSize;
				cin.ignore(10000, '\n');
			}

			if (numTest == 6) {
				cout << "Return code: " << differ(test, nSize, second, snSize) << endl << endl;
			} 
			else if (numTest == 7) {
				cout << "Return code: " << subsequence(test, nSize, second, snSize) << endl << endl;
			}
			else if (numTest == 8) {
				cout << "Return code: " << lookupAny(test, nSize, second, snSize) << endl << endl;
			}
		}
	}
}