/*
written for cs31 w/ smallberg
@author: 104785497
@version: 13177102016
*/

#include <iostream>
#include <string>
using namespace std;

/*
Written before the main method in order to calculate the license fees
Only executed if the valid criteria are met.
Takes two variables, p and r, which are intended to be the country and revenue, respectively.
*/
double calculate(string p, int r)
{
	double result = 0;
	
	if (r <= 20)								// checks below 20
		result += r*0.181;
	else 
	{
		r -= 20;								// removes 20 if over for future calculations
		result += 20.*0.181;
		if (r <= 30)							// checks below 50 (30 for r-20)
			if (p == "UAE" || p == "Turkey")	// UAE or Turkey check
				result += r*.217;
			else
				result += r*.203;
		else
		{
			r -= 30;							// removes an additional 30 if over 50
			if (p == "UAE" || p == "Turkey")	// UAE or Turkey check
				result += 30.*.217;
			else
				result += 30.*.203;
			result += r*.23;
		}
	}

	return result;
}

void main()
{
	string prop, country;						// create all variables beforehand
	int revenue;
	double licenseFee;

	cout << "Identification: ";					// query identification
	getline(cin, prop);

	cout << "Expected revenue (in millions): ";	// query expected revenue
	cin >> revenue;
	cin.ignore(10000, '\n');

	cout << "Country: ";						// query country
	getline(cin, country);

	cout << "---" << endl;

	if (prop.length() == 0)						// error checking
		cout << "You must enter a property identification." << endl;
	else if (revenue < 0)
		cout << "The expected revenue must be nonnegative." << endl;
	else if (country.length() == 0)
		cout << "You must enter a country." << endl;
	else										// all criteria are met
	{
		licenseFee = calculate(country, revenue);
		cout.setf(ios::fixed);					// fixed point adjustment
		cout.precision(3);
		cout << "The license fee for " << prop << " is $" << licenseFee << " million." << endl;
	}
}