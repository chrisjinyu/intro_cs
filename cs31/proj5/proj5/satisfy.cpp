/*
cs31 2016 smallberg q1lec1
project 5
104785497
*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>				//used only for testing purposes
#include <cstring>
#include <cctype>
#include <cassert>				//for testing
using namespace std;

//Constants set, word length and doc length given by specification
const int MAX_WORD_LENGTH = 20;
const int MAX_DOC_LENGTH = 200;

//helper methods
/*All in C String to lower case*/	void lowerString(char c[]);
/*Remove an index for char[][]*/	void removeIndex(char c[][MAX_WORD_LENGTH + 1], int n, int index);
/*Remove an index for int*/			void removeIndex(int a[], int n, int pos);

//checks satisfied requirements for document
int isSatisfied(const char word1[MAX_WORD_LENGTH + 1],
	const char word2[MAX_WORD_LENGTH + 1],
	const int distance,
	const char doc[][MAX_DOC_LENGTH + 1],
	const int length);

int normalizeRules(char word1[][MAX_WORD_LENGTH + 1],
	char word2[][MAX_WORD_LENGTH + 1],
	int distance[],
	int nRules)
//FUNCTION STARTS HERE//
{
	int origRules = nRules;				//for later use when removing to ensure all needed are shifted to "remove" from an array
	if (nRules < 0)						
		nRules = 0;						//for returning purposes, if it is negative, then there are 0 "normal" rules
	int removal;						//mark for removal
	for (int i = 0; i < nRules; i++)
	{
		lowerString(word1[i]);
		lowerString(word2[i]);			//set everything to lowercase
	}
	for (int i = 0; i < nRules; i++)	//check ALL rules
	{
		removal = 0;												//for each loop, removal must be set to false
		if (distance[i] <= 0)
			removal = 1;											//negative distance is grounds for removal
		else if (strlen(word1[i]) == 0 || strlen(word2[i]) == 0)		//remove if empty string
				removal = 1;
		else{
			for (int j = 0; j < strlen(word1[i]); j++) {
				if (!isalpha(word1[i][j]))
					removal = 1;									//remove if word1 has any non-alphabetic characters
			}
			for (int j = 0; j < strlen(word2[i]); j++) {
				if (!isalpha(word2[i][j]))
					removal = 1;									//remove if word2 has any non-alphabetic characters
			}

			if (!removal) {											//if no non-alphabetic characters, begin checking for duplicates through ALL remaining rules
				for (int j = 0; j < nRules; j++)
					if (i != j && 
						((strcmp(word1[i], word1[j]) == 0 && strcmp(word2[i], word2[j]) == 0) || 
						 (strcmp(word1[i], word2[j]) == 0 && strcmp(word2[i], word1[j]) == 0)) && 
						distance[i] <= distance[j])					//if the index is different, duplicate words (accounts for switched words), and shorter distance
						removal = 1;								//mark for removal
			}
		}
		if (removal) {								//removal!
			removeIndex(word1, origRules, i);
			removeIndex(word2, origRules, i);
			removeIndex(distance, origRules, i);					//all three of these methods shifts everything into the position i
			nRules--;												//nRules is decremented to account for removed value
			i--;													//i is decremented to maintain index
		}
	}
	return nRules;													//nRules is based on origRules - # of removed
}

//precondition:	word1, word2, and distance are a set of normalized rules
int calculateSatisfaction(const char word1[][MAX_WORD_LENGTH + 1],
	const char word2[][MAX_WORD_LENGTH + 1],
	const int distance[],
	int nRules,
	const char document[])
//FUNCTION STARTS HERE//
{
	int satisfied = 0;

/*REMOVE SPACES and NON-ALPHA*/
	int procLength = 0;												//for counting number of processed characters
	char processedDoc[MAX_DOC_LENGTH + 1];							//initially process document by removing duplicate spaces, non-alphabetic characters
	for (int i = 0; i < strlen(document); i++) {
		if (isalpha(document[i])) {
			processedDoc[procLength] = tolower(document[i]);		//add lowercase
			procLength++;											//increment character count
		}
		if (document[i] == ' ') {									//duplicate spaces
			if (procLength != 0 && processedDoc[procLength - 1] != ' ') { //if the previous is a space or is on first character, do not add
				processedDoc[procLength] = tolower(document[i]);		
				procLength++;										//increment character count
			}
		}
	}
	processedDoc[procLength] = '\0';								//exit character required, otherwise will consist of weird stuff

				//cout << "Your new document: ";
				//for (int i = 0; i < procLength - 1; i++)
				//	cout << processedDoc[i];
				//cout << processedDoc[procLength - 1] << endl;

/*CONVERT TO ARRAY OF C STRINGS*/
	int wordIndex = 0;												//for counting the # of words
	char doc[MAX_DOC_LENGTH][MAX_DOC_LENGTH + 1];					//initialize new array of C-Strings with maximum word count MAX_DOC_LENGTH and maximum word length MAX_DOC_LENGTH+1, just in case your test case has a single 200 character long word
	char tempWord[MAX_DOC_LENGTH + 1];								//temporary word used to fill in the above array, cut off with \0 when needed
	for (int i = 0, j = 0; i <= procLength; i++) {					//iterate through processedDoc, j is used to traverse tempWord
		if (processedDoc[i] != ' ' && processedDoc[i] != '\0') {	//is part of a word (not space & not exit char)
			tempWord[j] = processedDoc[i];							
			j++;													//sets the tempWord index to the current character
		}
		else {														//hey, its a space or exit character
			tempWord[j] = '\0';										//finish the C string
			j = 0;													//reset j for a new word
			if (strlen(tempWord)!=0) {								//if the tempWord isn't just an empty "", otherwise will mess up the actual calculation
				strcpy(doc[wordIndex], tempWord);					//copy tempWord into the doc array
				wordIndex++;										//increment word count
			}
		}
	}

				//cout << "Your array: {";
				//for (int i = 0; i < wordIndex - 1; i++)
				//	cout << "\"" << doc[i] << "\"" << ", ";
				//cout << "\"" << doc[wordIndex - 1] << "\"" << "}" << endl;

/*COUNT THE NUMBER OF SATISFIED RULES*/
	for (int i = 0; i < nRules; i++) {
		satisfied += isSatisfied(word1[i], word2[i], distance[i], doc, wordIndex);	//isSatisfied returns 1 if satisfied, 0 if not, so incrementation works as needed
	}																				//uses word1[i], word2[i], and distance[i] to isolate each rule, then searches the document w/ the word count limiter
	return satisfied;
}

int isSatisfied(const char word1[MAX_WORD_LENGTH + 1],
	const char word2[MAX_WORD_LENGTH + 1],
	const int distance,
	const char doc[][MAX_DOC_LENGTH + 1],
	const int length
)
{
	for (int i = 0; i < length; i++) {	//iterate through the processed document
		if (strcmp(doc[i], word1) == 0) {							//found word1
			for (int j = i - distance; j <= i + distance; j++) {	//iterate through i +/- distance inclusive
				if (j >= 0 && j < length && j != i) {				//making sure no out of bounds
					if (strcmp(doc[j], word2) == 0) {				//congrats you found the other word
						return 1;
					}
				}
			}
		}

	/*not even required, im silly*/
		//if (strcmp(doc[i], word2) == 0) {							//found word2
		//	for (int j = i - distance; j <= i + distance; j++) {
		//		if (j >= 0 && j < length && j != i) {
		//			if (strcmp(doc[j], word1) == 0) {				//exact same as above, just with words switched around
		//				return 1;
		//			}
		//		}
		//	}
		//}
	}
	return 0;
}

void lowerString(char c[])
{
	for (int i = 0; i < strlen(c); i++)
		c[i] = tolower(c[i]);			//go through all the C string, lowercase it
}

void removeIndex(char c[][MAX_WORD_LENGTH + 1], int n, int index)
{
	for (int i = index; i < n - 1; i++)
		strcpy(c[i], c[i + 1]);			//shift everything into the index position

	/*forgot strcpy exists*/
	//for (int j = 0; j < MAX_WORD_LENGTH - 1; j++)
	//	c[i][j] = c[i + 1][j];
}

//overloaded
void removeIndex(int a[], int n, int index) {
	for (int i = index + 1; i < n; i++)
		a[i - 1] = a[i];				//shift everything into the index position
}

/*TESTING PURPOSES ONLY*/
int main()
{
	//NORMALIZE TESTING
	//const MAX_WORD_LENGTH = 20;		already set at start of program
	const int TEST_NRULES1 = 21;

	char test1[TEST_NRULES1][MAX_WORD_LENGTH + 1] = {
		"ABc",			"aBc",		"pooh",				"eeyore",			"frEe",
		"hun-dred",		"eerf",		"acrewood",			"hundReD",			"sAnderz",
		"kanga",		"PIGLET",	"themostwonderful",	"tiggersisthat",	"wonderful",
		"bounNcy",		"boUNcy",	"Funn",				"fun",				"fun",			"otT"
	};

	char test2[TEST_NRULES1][MAX_WORD_LENGTH + 1] = {
		"ott",			"otT",		"tigg3r",			"rab bit",			"eErf",
		"acrewood",		"FREE",		"hundred",			"aCREwooD",			"Sanderz",
		"owl",			"roO",		"thingabout",		"tiggersare",		"things",
		"BoUncy",		"Bounncy",	"fun",				"fun",				"",				"aBc"
	};
	int testDist[TEST_NRULES1] = {
		3,				2,			5,					50,					6,
		12,				9,			1,					3,					1,
		0,				-3,			4,					1,					4,
		1,				2,			5,					7,					4,				1
	};

	int newRules = normalizeRules(test1, test2, testDist, 0);			//should be 0, array should also be unchanged
	assert(newRules == 0);
	newRules = normalizeRules(test1, test2, testDist, 0);				//should still be 0, array should also be unchanged
	assert(newRules == 0);
	newRules = normalizeRules(test1, test2, testDist, TEST_NRULES1);	//should be 10, array is changed as follows:
	assert(newRules == 10);

	/*
	all are lowercase, ignoring anything in indeces after newRules-1

	NEW test1:
	{	"abc",			"eerf",		"hundred",		"sanderz",	"themostwonderful",
		"tiggersisthat","wonderful","bouncy",		"funn",		"fun"				}

	NEW test2:
	{	"ott",			"free",		"acrewood",		"sanderz",	"thingabout",
		"tiggersare",	"things",	"bounncy",		"fun",		"fun"				}

	NEW testDist:
	{	3,				9,			3,				1,			4,
		1,				4,			2,				5,			7					}

	Removed indeces: {1, 2, 3, 4, 5, 7, 10, 11, 15, 19}
	Reasons:
	1:	duplicate found at index 0, testDist[1]<testDist[0]
	2:	non-alphabetic character found at test2[2][4], '3'
	3:	non-alphabetic character found at test2[3][3], ' '
	4:	duplicate found at index 6, testDist[4]<testDist[6]
	5:	non-alphabetic character found at test1[5][3], '-'
	7:	duplicate found at index 8, testDist[7]<testDist[8]
	10:	testDist[10] is not positive
	11:	testDist[11] is not positive
	15:	duplicate found at index 16, testDist[15]<testDist[16]
	19:	empty string found in test2[19]
	*/

	//SATISFACTION TESTING
	//also notable: checks that the arrays have actually been changed by normalizeFunction()
	//const int MAX_DOC_LENGTH = 200;		//already set at start of program

	char doc1[MAX_DOC_LENGTH + 1] =
		"Christopher Robin likes FREE things and he loves to write it as \"eerf\" at times. Sometimes he goes to sanderz SA*nderz place to have bounncy fun at the fun house of fun.";	//171

	char doc2[MAX_DOC_LENGTH + 1] =
		"Deep in the hundred acrewood--------thisisntevencorrect justkiddingitactuallyis ACREWOOD";	//88

	char doc3[MAX_DOC_LENGTH + 1] =
		"The.most.wonderful things about tiggers,is.that tiggers.are wonderful tings! they're bouncy bouncey boUnnCy funn funnn funnnn funnnnn fun! Th3eMos3twon;derful thingabo!ut! tiggers is I'm the only one!";	//199

	char doc4[MAX_DOC_LENGTH + 1] =
		"abc giveth and taketh none but ott stands for 123 which is to say that you can't necessarily have too much fun whenever 2020 rolls around since its never fun to get abc things for o-t-t f you know";	//196

	char doc5[MAX_DOC_LENGTH + 1] =
		"dfjsljivoakdjslkvjadslkjflJDFKLJFldsfjlkfJkfjfwoilkMcxvoJCOXIOQjpqijPJVDSPIVjPIDSJVLKXZJVlkcJLEIEJlckmKXCMZCLMceimcKMCISOjfjQPJPQKXzlcPQJPcLKJCACJXKCZCJKEojKPQCJXCLKJCZPCjqpkjPAKJCPJCpakscjpsakjSPCJSP"; //200

	char doc6[MAX_DOC_LENGTH + 1] =
		"   given     nothing       at al   l  l     does SAnderz***sanderz sanderz ---59857329-----   fun is out of bounds from itself, since fun is 8 away from itself and not 7";	//169

	char doc7[MAX_DOC_LENGTH + 1] =
		""; //0

	char doc8[MAX_DOC_LENGTH + 1] =
		"lets have some fun free.eerf sanderz_sanderz themostwonderful-thingabout-;-tiggersisthat|tiggersare wonderful?things funfunn funfun fun! abcott"; //143

	assert(calculateSatisfaction(test1, test2, testDist, newRules, doc1) == 3);
	//Satisfied Rules: 	{1, 3, 9}
	//	1:	"free" and "eerf" are found 					9 words apart,	9<=testDist[1] or 9
	//		ignores quotation characters '\"' around "eerf"
	//	3:	"sanderz" and "sanderz" are found 				1 word apart,	1<=testDist[3] or 1
	//		duplicate words work
	//	9:	"fun" and "fun" are found						3 words apart,	3<=testDist[9] or 7
	//		are also found twice, but only satisfies the rule once
	//		duplicate words work

	assert(calculateSatisfaction(test1, test2, testDist, newRules, doc2) == 1);
	//Satisfied Rule:	{2}
	//	2:	"hundred" and "acrewood" are found	3 words apart,	3<=testDist[2] or 3
	//		despite there being symbols in the way, acrewood--------thisisntevencorrect still counts as acrewoodthisisntevencorrect, or one word

	assert(calculateSatisfaction(test1, test2, testDist, newRules, doc3) == 5);
	//Satisfied Rules:	{4, 5, 6, 7, 8}
	//	4:	"themostwonderful" and "thingabout" are found	1 word apart,	1<=testDist[4] or 4
	//		even when split by symbols and numbers but NOT spaces, they still count as single words.
	//	5:	"tiggersisthat" and "tiggersare" are found		1 word apart,	1<=testDist[5] or 1
	//		same test as above
	//	6:	"things" and "wonderful" are found				4 words apart,	4<=testDist[6] or 4
	//		reversed order still counts
	//	7:	"bouncy" and "bouncey" are found				2 words apart,	2<=testDist[7] or 2
	//		capitalization does not matter	
	//	8:	"funn" and "fun" are found						4 words apart,	4<=testDist[8] or 5


	assert(calculateSatisfaction(test1, test2, testDist, newRules, doc4) == 2);
	//Satisfied Rules:	{0, 9}
	//	0:	"abc" and "ott" are found		3 words apart,	3<=testDist[0] or 3
	//		internal separation of words by symbols does not interrupt the word, nor count as part of the word
	//	9:	"fun" and "fun" are found		7 words apart,	7<=testDist[9] or 7
	//		despite there being a "word" in between, "2020" does not actually count as a word since it is not alphabetic

	assert(calculateSatisfaction(test1, test2, testDist, newRules, doc5) == 0);
	//Reasons for dissatisfaction:
	//	None of the word pairs can be found in it.

	assert(calculateSatisfaction(test1, test2, testDist, newRules, doc6) == 0);
	//Reasons for dissatisfaction:
	//	Most notably, "sanderz" does NOT count itself in its word pair. Also, "fun" is 8 away from "fun", not 7 or lower

	assert(calculateSatisfaction(test1, test2, testDist, newRules, doc7) == 0);
	//Reasons for dissatisfaction:
	//	Empty string. No rule is found.

	assert(calculateSatisfaction(test1, test2, testDist, newRules, doc8) == 1);
	//Satisfied Rules:	{9}
	//	9:	"fun" and "fun" are found		7 (very long) words apart,	7<=testDist[9] or 7
	//		other rules (0, 1, 4, 5, 6) are not satisfied, since their words are not separated by spaces.

	//MISC. TESTS
	assert(calculateSatisfaction(test1, test2, testDist, 0, doc1) == 0);
	assert(calculateSatisfaction(test1, test2, testDist, 0, doc2) == 0);
	assert(calculateSatisfaction(test1, test2, testDist, 0, doc3) == 0);
	assert(calculateSatisfaction(test1, test2, testDist, 0, doc4) == 0);
	assert(calculateSatisfaction(test1, test2, testDist, 0, doc5) == 0);
	assert(calculateSatisfaction(test1, test2, testDist, 0, doc6) == 0);
	assert(calculateSatisfaction(test1, test2, testDist, 0, doc7) == 0);
	assert(calculateSatisfaction(test1, test2, testDist, 0, doc8) == 0);
	//nRule = 0 should return no satisfied rules or 0, since no rules are processed
	assert(calculateSatisfaction(test1, test2, testDist, -1, doc1) == 0);
	assert(calculateSatisfaction(test1, test2, testDist, -83, doc2) == 0);
	assert(calculateSatisfaction(test1, test2, testDist, -98427, doc3) == 0);
	assert(calculateSatisfaction(test1, test2, testDist, -38, doc4) == 0);
	assert(calculateSatisfaction(test1, test2, testDist, -5, doc5) == 0);
	assert(calculateSatisfaction(test1, test2, testDist, -23, doc6) == 0);
	assert(calculateSatisfaction(test1, test2, testDist, -99, doc7) == 0);
	assert(calculateSatisfaction(test1, test2, testDist, -333, doc8) == 0);
	//nRule < 0 treated as 0, should return no satisfied rules, or 0

	//given testing
	const int TEST1_NRULES = 4;
	char test1w1[TEST1_NRULES][MAX_WORD_LENGTH + 1] = {
		"mad",       "deranged", "nefarious", "have"
	};
	char test1w2[TEST1_NRULES][MAX_WORD_LENGTH + 1] = {
		"scientist", "robot",    "plot",      "mad"
	};
	int test1dist[TEST1_NRULES] = {
		2,           4,          1,           13
	};
	assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
		"The mad UCLA scientist unleashed a deranged evil giant robot.") == 2);		// should be 2
	assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
		"The mad UCLA scientist unleashed    a deranged robot.") == 2);				//should be 2
	assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
		"The mad **** 2016 **** UCLA scientist unleashed a deranged evil giant robot.") == 2);
	assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
		"          That plot: NEFARIOUS!") == 1);
	assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
		"deranged deranged robot deranged robot robot") == 1);
	assert(calculateSatisfaction(test1w1, test1w2, test1dist, TEST1_NRULES,
		"That scientist said two mad scientists suffer from deranged-robot fever.") == 0);

	cout << "gj boi" << endl;
}