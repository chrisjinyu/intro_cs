#include <string>
#include <stack>

//#define TESTING

using namespace std;

bool precedence(char c1, char c2)
{
	switch (c2)
	{
	case '!':
	case '&':
		if (c1 == '!')
			return false;
		else
			return true;
	case '|':
		if (c1 == '|')
			return true;
		return false;
	}
}

void removeBlanks(string& s)
{
	string n;
	for (int i = 0; i < s.size(); i++)
		if (s[i] != ' ')
			n += s[i];
	s = n;
}

bool isOperator(char c)
{
	return c == '!' || c == '&' || c == '|';
}

bool isStandOperator(char c)
{
	return c == '&' || c == '|';
}

bool isDigit(char c)
{
	return c >= '0' && c <= '9';
}

bool isValidInfix(string infix)
{
	int open = 0, close = 0;
	if (infix.size() == 0 || isStandOperator(infix[0]) || isOperator(infix[infix.size() - 1]))
		return false;
	for (int i = 0; i < infix.size() - 1; i++)
	{
		char c = infix[i];
		char n = infix[i + 1];
		if (isDigit(c))
		{
			if (isDigit(n)) return false;
		}
		else if (isOperator(c))
		{
			if (isStandOperator(n)) return false;
			if (n == ')') return false;
		}
		else if (c == '(')
		{
			open++;
			if (n == ')') return false;
			if (isStandOperator(n)) return false;
		}
		else if (c == ')')
		{
			close++;
		}
		else
			return false;
	}
	if (infix[infix.size() - 1] == ')')
		close++;
	if (close != open)
		return false;
	return true;
}

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
	removeBlanks(infix);
	if (!isValidInfix(infix))
		return 1;

	stack<char> opStack;
	string pf;
	for (int i = 0; i < infix.size(); i++)
	{
		char current = infix[i];
		if (isDigit(current))
			pf += current;
		else if (current == '(')
			opStack.push(current);
		else if (current == ')')
		{
			while (opStack.top() != '(')
			{
				pf += opStack.top();
				opStack.pop();
			}
			opStack.pop();
		}
		else if (isOperator(current))
		{
			while (!(opStack.empty() || opStack.top() == '(')
				&& precedence(current, opStack.top()))
			{
				pf += opStack.top();
				opStack.pop();
			}
			opStack.push(current);
		}
	}
	while (!opStack.empty())
	{
		pf += opStack.top();
		opStack.pop();
	}

	stack<bool> evalStack;
	for (int i = 0; i < pf.size(); i++)
	{
		char current = pf[i];
		if (current >= '0' && current <= '9')
			evalStack.push(values[current - '0']);
		else if (current == '&' || current == '|')
		{
			bool op2 = evalStack.top();
			evalStack.pop();
			bool op1 = evalStack.top();
			evalStack.pop();
			if (current == '&')
				evalStack.push(op1 && op2);
			else
				evalStack.push(op1 || op2);
		}
		else if (current == '!')
		{
			bool op1 = evalStack.top();
			evalStack.pop();
			evalStack.push(!op1);
		}
	}
	if (evalStack.empty())
		return 1;
	bool potentialResult = evalStack.top();
	int count = 0;
	while (!evalStack.empty())
	{
		evalStack.pop();
		count++;
	}
	if (count == 1)
	{
		postfix = pf;
		result = potentialResult;
		return 0;
	}
	else return 1;
}

#ifdef TESTING
#include <assert.h>
#include <iostream>

int main()
{
	bool ba[10] = {
		//  0      1      2      3      4      5      6      7      8      9
		true,  true,  true,  false, false, false, true,  false, true,  false
	};
	string pf;
	bool answer;

	assert(evaluate("2", ba, pf, answer) == 0 && answer);
	assert(evaluate("(3)", ba, pf, answer) == 0 && !answer);
	assert(evaluate("2&(3)", ba, pf, answer) == 0 && !answer);
	assert(evaluate("0 & !9", ba, pf, answer) == 0 && answer);
	assert(evaluate("!(7|8)", ba, pf, answer) == 0 && !answer);
	assert(evaluate("!7|8  ", ba, pf, answer) == 0 && answer);
	assert(evaluate("6|4&5 ", ba, pf, answer) == 0 && answer);
	assert(evaluate("1&!(9|1&1|9) | !!!(9&1&9)", ba, pf, answer) == 0 && answer);

	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" &&  answer);
	assert(evaluate("8|", ba, pf, answer) == 1);
	assert(evaluate("4 5", ba, pf, answer) == 1);
	assert(evaluate("01", ba, pf, answer) == 1);
	assert(evaluate("()", ba, pf, answer) == 1);
	assert(evaluate("2(9|8)", ba, pf, answer) == 1);
	assert(evaluate("2(&8)", ba, pf, answer) == 1);
	assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
	assert(evaluate("", ba, pf, answer) == 1);
	assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
		&& pf == "43!03&&|" && !answer);
	assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
	ba[2] = false;
	ba[9] = true;
	assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9"  &&  answer);
	assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);


	cout << "Passed all tests" << endl;
}
#endif