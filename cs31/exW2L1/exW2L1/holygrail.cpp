#include <iostream>
#include <string>
using namespace std;

int main()
{
	string name = "";
	int age;

	while (name.length() == 0)
	{
		cout << "What is your name? ";
		getline(cin, name);

	}

	cout << "How old are you? ";
	cin >> age;
	cin.ignore(1000, '\n');

	string quest;
	while (quest.length() == 0)
	{
		cout << "What is your quest? ";
		getline(cin, quest);
	}

	cout << "Hello, brave " << name << "!" << endl;
	cout << "You want " << quest << endl;
	cout << "If you live, next year you will be " << age + 1 << endl;

	return 0;
}