#include "newSequence.h"

Sequence::Sequence()
{
	m_sequence = new ItemType[DEFAULT_MAX_ITEMS];
	m_maxItems = DEFAULT_MAX_ITEMS;
	m_size = 0;
}

Sequence::Sequence(int size)
{
	m_sequence = new ItemType[size];
	m_maxItems = size;
	m_size = 0;
}

Sequence::Sequence(const Sequence &old)
{
	m_maxItems = old.m_maxItems;
	m_sequence = new ItemType[m_maxItems];
	for (int i = 0; i < m_maxItems; i++)
		m_sequence[i] = old.m_sequence[i];
	m_size = old.m_size;
}

Sequence& Sequence::operator=(const Sequence &old)
{
	if (&old != this)
	{
		delete[] m_sequence;
		m_maxItems = old.m_maxItems;
		m_sequence = new ItemType[m_maxItems];
		for (int i = 0; i < m_maxItems; i++)
			m_sequence[i] = old.m_sequence[i];
		m_size = old.m_size;
	}
	return (*this);
}

Sequence::~Sequence()
{
	delete[] m_sequence;
}

bool Sequence::empty() const
{
	return m_size == 0;
}

int Sequence::size() const
{
	return m_size;
}

bool Sequence::insert(int pos, const ItemType& value)
{
	if (pos < 0 || pos > size() || size() == m_maxItems)
		return false;
	m_size++;
	for (int i = size() - 1; i > pos; i--)
	{
		m_sequence[i] = m_sequence[i - 1];
	}
	m_sequence[pos] = value;
	return true;
}

int Sequence::insert(const ItemType& value)
{
	if (size() == m_maxItems)
		return -1;
	int p = 0;
	for (p = 0; value > m_sequence[p] && p < size(); p++) {}
	m_size++;
	for (int i = size() - 1; i > p; i--)
	{
		m_sequence[i] = m_sequence[i - 1];
	}
	m_sequence[p] = value;
	return p;
}

bool Sequence::erase(int pos)
{
	if (pos < 0 || pos >= size())
		return false;
	for (int i = pos; i < size(); i++)
	{
		m_sequence[i] = m_sequence[i + 1];
	}
	m_size--;
	return true;
}

int Sequence::remove(const ItemType& value)
{
	int numRemoved = 0;
	for (int i = size() - 1; i >= 0; i--)
	{
		if (value == m_sequence[i])
		{
			erase(i);
			numRemoved++;
		}
	}
	return numRemoved;
}

bool Sequence::get(int pos, ItemType& value) const
{
	if (pos < 0 || pos >= size())
		return false;
	value = m_sequence[pos];
	return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
	if (pos < 0 || pos >= size())
		return false;
	m_sequence[pos] = value;
	return true;
}

int Sequence::find(const ItemType& value) const
{
	int p = 0;
	for (p = 0; m_sequence[p] != value && p < size(); p++) {}
	if (p < size())
		return p;
	else
		return -1;
}

void Sequence::swap(Sequence& other)
{
	Sequence temp = other;
	other = *this;
	*this = temp;

	/*int tempSize = other.m_maxItems;
	other.m_maxItems = m_maxItems;
	m_maxItems = tempSize;

	int pos = 0;
	ItemType temp1, temp2;
	for (pos = 0; pos < size() && pos < other.size(); pos++)
	{
		get(pos, temp1);
		other.get(pos, temp2);
		set(pos, temp2);
		other.set(pos, temp1);
	}
	if (size() > other.size())
	{
		int maxSize = size();
		for (int p = other.size(); p < maxSize; p++)
		{
			get(pos, temp1);
			other.insert(p, temp1);
			erase(pos);
		}
	}
	else if (size() < other.size())
	{
		int maxSize = other.size();
		for (int p = size(); p < maxSize; p++)
		{
			other.get(pos, temp1);
			insert(p, temp1);
			other.erase(pos);
		}
	}*/
}