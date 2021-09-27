#include <iostream>
using namespace std;

int main()
{
	cout << "How many hours did you work ? ";
	double hoursWorked;
	cin >> hoursWorked;
	cout << "What is your hourly rate of pay ? ";
	double rateOfPay;
	cin >> rateOfPay;

	double totalPay = hoursWorked*rateOfPay;

	cout.setf(ios::fixed);       // see pp. 32-33 in Savitch 6/e
	cout.precision(2);

	cout << "You earned $" << totalPay << endl;
	cout << "$" << totalPay / 10. << " will be withheld." << endl;
}
