/*
* Christopher Wang
* UID: 604781251
* CS31
*/


#include <iostream>
#include <string>
#include <cctype>
#include <cassert>
using namespace std;

bool hasCorrectSyntax(string pollData);								//method prototypes
bool processForecasts(string pollData);
bool isValidUppercaseStateCode(string stateCode);
int countVotes(string pollData, char party, int& voteCount);

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

int main() {
	return testing();
}

bool hasCorrectSyntax(string pollData) {
	for (size_t i = 0; i < pollData.size(); i++) {										//ensures that there are only alphanumeric chars in pollData
		if ((!isalpha(pollData[i]) && !isdigit(pollData[i])) || pollData[i] == ' ')
			return false;
	}
	if (pollData.size() == 0)		//if string is empty, return true
		return true;
	return processForecasts(pollData);	//passes pollData to return true if it is a valid pollData string
}

bool processForecasts(string pollData) {
	size_t pos = 0;													//initialization of local variables for different parts of the state forecast
	string state = "";
	string votes = "";
	char party = NULL;
	while (pos != pollData.size()) {								//Begins the process of verifying that the string contains complete & correct forecasts
		for (int i = 0; i < 2; i++) {
			if (pos < pollData.size() && isalpha(pollData[pos])) {	//This loop ensures that the first two chars of the forecast
				state += toupper(pollData[pos]);					//are alpha chars and that the code does not attempt to access out of bounds
				pos++;
			}
			else
				return false;
		}

		if (!isValidUppercaseStateCode(state))						//verifies that the first two chars stored in state are valid state codes
			return false;

		if (pos + 2 < pollData.size() && isdigit(pollData[pos]) && isdigit(pollData[pos + 1]) && isalpha(pollData[pos + 2])) { //Checks that the next 3 chars
			votes = votes + pollData[pos] + pollData[pos + 1];															//are two digits and a letter,
			party = pollData[pos + 2];																					//and that the code does not go
			pos += 3;																									//out of bounds
		}
		else if (pos + 1 < pollData.size() && isdigit(pollData[pos]) && isalpha(pollData[pos + 1])) {					//If the vote number is only 1 digit,
			votes += pollData[pos];																					//code checks if it is a digit followed
			party = pollData[pos + 1];																				//by a alpha character
			pos += 2;
		}
		else
			return false;																							//returns false in any other case
		state = "";																									//resets all the local variables and
		votes = "";																									//moves on to the next state forecast
		party = NULL;
	}
	return true;
}

bool isValidUppercaseStateCode(string stateCode) {																	//code taken from the project website
	const string codes =
		"AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
		"KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC."
		"ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
	return (stateCode.size() == 2 &&
		stateCode.find('.') == string::npos  &&  // no '.' in stateCode
		codes.find(stateCode) != string::npos);  // match found
}

int countVotes(string pollData, char party, int& voteCount) {
	if (!hasCorrectSyntax(pollData))							//if the syntax is incorrect, returns 1
		return 1;
	if (!isalpha(party))										//if the party code is not an alpha char, returns 3
		return 3;
	size_t pos = 0;												//initialization of local variables for vote counting
	string state = "";
	string votes = "";
	char partyCode = NULL;
	while (pos != pollData.size()) {
		for (int i = 0; i < 2; i++) {
			if (pos < pollData.size()) {						//making sure that pos will not go out of bounds
				state += toupper(pollData[pos]);				//no need to check if the chars are alpha and if the state code is valid
				pos++;
			}
		}

		if (isdigit(pollData[pos]) && isdigit(pollData[pos + 1]) && isalpha(pollData[pos + 2]) && pos + 2 < pollData.size()) { //very similar code that
			votes = votes + pollData[pos] + pollData[pos + 1];																//determines which scenario
			partyCode = pollData[pos + 2];																					//is applicable in terms of
			pos += 3;																										//the vote count
		}
		else if (isdigit(pollData[pos]) && isalpha(pollData[pos + 1]) && pos + 1 < pollData.size()) {
			votes += pollData[pos];
			partyCode = pollData[pos + 1];
			pos += 2;
		}

		int stateVotes = stoi(votes, nullptr, 10);		//translates votes from a string to a int in decimal base
		if (toupper(partyCode) == toupper(party)) {		//if the party code given matches the one read, voteCount is adjusted accordingly
			if (!stateVotes == 0)
				voteCount += stateVotes;
			else
				return 2;								//returns 2 if the vote count is 0
		}

		state = "";										//resets the local variables for the next state forecast
		votes = "";
		partyCode = NULL;
	}
	return 0;											//what to return when the string is fully processed
}