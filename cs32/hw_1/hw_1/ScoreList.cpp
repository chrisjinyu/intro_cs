#include "ScoreList.h"

ScoreList::ScoreList(){}

bool ScoreList::add(unsigned long score) 
{
	if (score < 0 || score > 100)			//for some reason, linux g++ compiler states that score cannot possibly be below 0
		return false;
	if(m_sequence.insert(score) != -1)
		return true;
	return false;
}

bool ScoreList::remove(unsigned long score)
{
	int pos = m_sequence.find(score);
	if (pos == -1)
		return false;
	m_sequence.erase(m_sequence.find(score));
	return true;
}

int ScoreList::size() const
{
	return m_sequence.size();
}

unsigned long ScoreList::minimum() const
{
	if (size() == 0)
		return NO_SCORE;
	unsigned long minimum;
	unsigned long value;
	m_sequence.get(0, minimum);
	for (int i = 1; i < size(); i++)
	{
		m_sequence.get(i, value);
		if (value < minimum)
			minimum = value;
	}
	return minimum;
}

unsigned long ScoreList::maximum() const
{
	if (size() == 0)
		return NO_SCORE;
	unsigned long maximum;
	unsigned long value;
	m_sequence.get(0, maximum);
	for (int i = 1; i < size(); i++)
	{
		m_sequence.get(i, value);
		if (value > maximum)
			maximum = value;
	}
	return maximum;
}