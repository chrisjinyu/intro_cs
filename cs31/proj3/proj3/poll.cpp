/*
	cs31 smallberg
	project 3
	104785497
*/

#include <iostream>				//used only for testing purposes
#include <string>
#include <cctype>
#include <cassert>				//for testing
using namespace std;

/* function prototypes */
bool hasCorrectSyntax(string pollData);
int countVotes(string pollData, char party, int& voteCount);

bool isStateCode(string s);
int testing();
void assertionTest();

//not used in actual grading, testing purposes only
int main() {
	return testing();
}

//checks if the string pollData is a valid, readable poll data string
bool hasCorrectSyntax(string pollData) {
	if (pollData.length() == 0)				//empty string is valid
		return true;
	if (pollData.length() < 4 || !isalpha(pollData[pollData.length() - 1]) || !isdigit(pollData[pollData.length() - 2]))
		return false;						//checks base cases: length, alphabetic last character, etc.

	int block = 0;							//block ID to identify which transversal is used, 0 for state code, 1 for digits, 2 for party code

	for (int i = 0; i < pollData.length() - 1; i++) {	//last character already checked
		if (block == 0) {					//state
			if (pollData.length() - i < 4 || !isalpha(pollData[i]))
				return false;					//possible length check (state code cannot be too close to the end); if statecode transversal does not start with alphabetic, then it is invalid
			string test;
			test += pollData[i];
			test += pollData[i + 1];			//adds both characters in stateCode to test string
			if (!isStateCode(test))
				return false;
			i++;								//increment 2 (total, 1 in for loop +1 here)
			block = 1;							//cycle digit
		}
		else if (block == 1) {				//digits
			if (pollData.length() - i < 2 || !isdigit(pollData[i]))
				return false;					//possible length check (digits cannot be too close to the end); if this position is not a digit (follows state code), then it is invalid
			if (isdigit(pollData[i + 1]))
				i++;							//increment 2 only if second digit is present
			block = 2;							//cycle party
		}
		else if (block == 2) {				//party
			if (!isalpha(pollData[i]))
				return false;					//character following digit must be alphabetic
			block = 0;							//cycle state code
		}
	}
	if (block != 0)							//if it somehow didn't end on the state code block
		return true;
	return false;
}

//adds the votes for a party (provided as an argument) to voteCount, returns error codes
// 0 : no error
// 1 : syntax error
// 2 : zero electoral votes is given for at least one state
// 3 : party given as argument is invalid
int countVotes(string pollData, char party, int& voteCount) {
	if (!hasCorrectSyntax(pollData))					//incorrect syntax must return 1
		return 1;
	if (!isalpha(party))								//non-alphabetic character must return 2
		return 3;

	int votes = 0, blockVotes = 0;
	party = toupper(party);								//non-floating upper/lower for party
	string digits;

	for (int i = 3; i < pollData.length(); i++) {		//begins at a minimum index for party code
		digits = "0";								//avoids errors in parsing

		/*newer method that is too easy to use and doesn't work on old compilers before C++11*/
		//if (pollData[i] == party || pollData[i] == tolower(party)) {
		//	if (isdigit(pollData[i - 1])) {
		//		if (isdigit(pollData[i - 2]))
		//			digits += pollData[i - 2];				//if the digit sequence is two characters long, adds the first one
		//		digits += pollData[i - 1];					//adds the guaranteed digit to the string
		//		blockVotes = std::stoi(digits);				//parses string to int

		if (isalpha(pollData[i])) {
			if (isdigit(pollData[i - 1])) {
				if (isdigit(pollData[i - 2]))
					blockVotes += 10 * ((int)pollData[i - 2] - 48);				//if the digit sequence is two characters long, adds the first one
				blockVotes += (int)pollData[i - 1] - 48;					//adds the guaranteed digit to the string

				if (blockVotes == 0)
					return 2;
				if (pollData[i] == party || pollData[i] == tolower(party))
					votes += blockVotes;
				blockVotes = 0;
			}
		}
	}
	voteCount = votes;
	return 0;
}

//statecode check, uses list from cs31 website, altered to check both upper/lower case
bool isStateCode(string s) {
	const string codes =
		"AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."		//list of state codes separated by periods for string.find use
		"KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC."		//list from CS31 website
		"ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
	string u;
	for (int i = 0; i < s.length(); i++)
		u += toupper(s[i]);											//does not matter if upper or lower, upper is chosen since list was readily available
	return(u.size() == 2 && u.find('.') == string::npos && codes.find(u) != string::npos);
}

//default sample assertion test
void assertionTest() {
	assert(hasCorrectSyntax("TX38RCA55D"));
	assert(!hasCorrectSyntax("MX38RCA55D"));
	int votes;
	votes = -999;    // so we can detect whether countVotes sets votes
	assert(countVotes("TX38RCA55DMs6rnY29dUT06L", 'd', votes) == 0 && votes == 84);
	votes = -999;    // so we can detect whether countVotes sets votes
	assert(countVotes("TX38RCA55D", '%', votes) == 3 && votes == -999);
	cout << "All tests succeeded" << endl;
}

//manual input testing
int testing() {
	cout << endl << "SINGLE TESTING\n----------------------------\n";
	int dVote = 0, rVote = 0, lVote = 0, gVote = 0, tVote = 0;
	string input;
	int x = 0;				//0 single, 1 continuous
	while (x == 0) {
		cout << "Please enter a test Poll Data string.\n\n";
		getline(cin, input);												//receives input for testing

		if (input == "0" || input == "no" ||
			input == "quit" || input == "exit")								//exit test function
			return 0;

		//if (input == "default") {											//sample testing function
		//	assertionTest();
		//	return 0;
		//}

		if (input == "continuous") {										//moves to continuous testing
			x = 1;
			cout << endl << "CONTINUOUS TESTING\n----------------------------\n";
			break;
		}

		if (hasCorrectSyntax(input))										//syntax check
			cout << "Correct syntax." << endl << endl;
		else
			cout << "Incorrect syntax." << endl << endl;

		dVote = 0, rVote = 0, lVote = 0, gVote = 0, tVote = 0;
		cout << "VOTE RESULTS:\n----------------------------\n";
		int dCode = countVotes(input, 'd', dVote), rCode = countVotes(input, 'r', rVote), lCode = countVotes(input, 'l', lVote),
			gCode = countVotes(input, 'g', gVote), tCode = countVotes(input, '*', tVote);	//tCode intentionally set to '*'
		cout << "Return Code: " << dCode << " | Democrat: " << dVote << endl;
		cout << "Return Code: " << rCode << " | Republican: " << rVote << endl;
		cout << "Return Code: " << lCode << " | Libertarian: " << lVote << endl;
		cout << "Return Code: " << gCode << " | Green: " << gVote << endl;
		cout << "Return Code: " << tCode << " | test_party: " << tVote << endl << endl;
	}
	while (x == 1) {														//exactly the same, just doesn't set all vote values to 0
		cout << "Please enter a test Poll Data string.\n";
		getline(cin, input);

		if (input == "0" || input == "no" ||
			input == "quit" || input == "exit")
			return 0;
		if (input == "single") {											//moves to single testing
			testing();														//ez recursive text method
			return 0;
		}

		//if (input == "default") {
		//	assertionTest();
		//	return 0;
		//}

		if (hasCorrectSyntax(input))
			cout << "Correct syntax." << endl << endl;
		else
			cout << "Incorrect syntax." << endl << endl;

		cout << "VOTE RESULTS:\n----------------------------\n";
		int dCode = countVotes(input, 'd', dVote), rCode = countVotes(input, 'r', rVote), lCode = countVotes(input, 'l', lVote),
			gCode = countVotes(input, 'g', gVote), tCode = countVotes(input, '*', tVote);
		cout << "Return Code: " << dCode << " | Democrat: " << dVote << endl;
		cout << "Return Code: " << rCode << " | Republican: " << rVote << endl;
		cout << "Return Code: " << lCode << " | Libertarian: " << lVote << endl;
		cout << "Return Code: " << gCode << " | Green: " << gVote << endl;
		cout << "Return Code: " << tCode << " | test_party: " << tVote << endl << endl;
	}
}