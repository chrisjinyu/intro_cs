#include <iostream>
#include <string>
using namespace std;

void mystery2(int &a, int b, int &c)
{
	cout << a << " " << b << " " << c << endl;
	a = b + c;
	cout << a << " " << b << " " << c << endl;
	b = a + c;
	cout << a << " " << b << " " << c << endl;
	c = a + b;
	cout << a << " " << b << " " << c << endl;
}


/*int main()
{
	int a, b, c;
	a = 1;
	b = 2;
	c = 3;

	mystery2(b, c, a);

	cout << a << " " << b << " " << c << endl;
}
*/
