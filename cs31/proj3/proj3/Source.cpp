#include <iostream>
using namespace std;

void removeS(char* c)
{
	while (*c != '\0')
	{
		if (*c == 's' || *c == 'S')
		{
			for (int i = 0; *(c + i) != '\0'; i++)
			{
				*(c + i) = *(c + i + 1);
			}
		}
		else
			c++;
	}
}

int main()
{
	char msg[50] = "She'll be a massless princess.";
	removeS(msg);
	cout << msg << endl;  // prints   he'll be a male prince.
}