#include <iostream>
using namespace std;

int main()
{
	const char* str = "C+ B+ B- A C A- A C- B+ D B";
	int count = 0;
	const char* p;
	for (p = str; *p != '\0' && count < 6; p++)
		if (*p == ' ')
			count++;
	for (; *p != '\0' && *p != ' '; p++)
		cout << *p;
	cout << '\n';
}