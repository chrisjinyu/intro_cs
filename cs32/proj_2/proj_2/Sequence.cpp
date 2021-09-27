/*
	CS32 PROJECT 2
	Christian Yu
	104785497
	CS32-2 w/ Smallberg
*/

#include "Sequence.h"

//POSTCONDITION: empty Sequence is created
Sequence::Sequence()
{
	m_head = nullptr;										//initialize both head and tail to nullptr, empty
	m_tail = nullptr;
	m_size = 0;
}

//POSTCONDITION: constructed Sequence contains exact same values in the same order, with different addresses and same size
Sequence::Sequence(const Sequence& old)
{
	m_size = 0;												//initialize an empty Sequence
	m_head = nullptr;
	m_tail = nullptr;
	Node* p = old.m_head;									//point to first in old array
	for (int i = 0; p != nullptr && i < old.m_size; i++)	//iterate through old Sequence, inserting each value sequentially
	{
		insert(i, p->value);
		p = p->next;
	}
}

//POSTCONDITION: current Sequence contains exact same values in the same order, with different addresses and same size
Sequence& Sequence::operator=(const Sequence& other)
{
	if (&other != this)										//check to see if pointers are not equal to itself
	{
		deleteList();										//set list to empty (head, tail = nullptr; m_size = 0)
		Node* p = other.m_head;								//begins iteration through the other Sequence, inseting values sequentially
		for (int i = 0; p != nullptr && i < other.m_size; i++)
		{
			insert(i, p->value);
			p = p->next;
		}
	}
	return (*this);											//returns itself as the result of the operator=
}

//DESTRUCTOR
Sequence::~Sequence()
{
	deleteList();											//deletes all Nodes, sets m_head, m_tail = nullptr; m_size=0
}

bool Sequence::empty() const
{
	return m_head == nullptr &&					//return all empty-case boolean expressions
		m_tail == nullptr &&
		m_size == 0;
}

int Sequence::size() const
{
	return m_size;								//trivial
}

//POSTCONDITION: if pos is within bounds, value is inserted at pos
bool Sequence::insert(int pos, const ItemType& value)
{
	if (pos < 0 || pos > m_size)				//boundary check
		return false;
	Node* p;									//Node pointer initialized to keep track of location of insertion
	if (pos == 0)
	{											//In the case that the first position is the argument,
		p = m_head;								//the Node pointer p is set to the current m_head (even if it is nullptr)
		m_head = new Node;						//m_head is reassigned to point to a new, blank Node
		m_head->value = value;					//m_head is given the value to be inserted
		m_head->next = p;						//m_head's next is set to the pointer to the node @ p
		m_head->back = nullptr;					//m_head's back is set to nullptr (since it is the head)
		if (m_size == 0)						//if the Sequence was originally empty,
			m_tail = m_head;					//m_tail should be equal to m_head at m_size = 1 (done at the end of the function)
		else if (p != nullptr)					//if m_size > 1, then p shouldn't be equal to nullptr
			p->back = m_head;					//give the pointer ahead of m_head a backwards value pointing to m_head
	}
	else if (pos == m_size)
	{											//In the case that the last position is the argument,
		p = m_tail;								//the Node pointer p is set to the current m_tail
		m_tail = new Node;						//m_tail is reassigned to point to a new, blank Node
		m_tail->value = value;					//m_tail is given the value to be inserted
		m_tail->next = nullptr;					//m_tail's next is set to nullptr (end pointer)
		if (m_size != 0 && p != nullptr)		//if m_size > 1, and p isn't nullptr
			p->next = m_tail;					//give the pointer p a next value of m_tail
		m_tail->back = p;						//set m_tail's back to p, even if nullptr
	}
	else
	{											//in neither case, insert into the middle
		Node* temp = new Node;					//create a new temporary pointer set to a new Node object
		temp->value = value;					//assign the temp the value
		if (pos <= m_size - pos)				//determine which would be the fastest direction to add (this is from the head)
		{										//(ONLY RELEVANT FOR VERY LONG LISTS)
			p = m_head;							//start from the head
			for (int i = 0; i < pos - 1; i++)
				p = p->next;					//iterate through until you reach the position-1 with the Node pointer
		}
		else
		{
			p = m_tail;							//start from the tail
			for (int i = m_size - 1; i > pos - 1; i--)
				p = p->back;					//iterate from the back until reaching the position-1 with Node pointer
		}
		temp->next = p->next;					//temp is given the place of p, after it
		if (temp->next != nullptr)
			temp->next->back = temp;			//the value after temp points back to temp
		temp->back = p;
		p->next = temp;							//p is placed before temp
	}
	m_size++;									//INCREMENT SIZE
	return true;								//returns true on successful placement
}

//POSTCONDITION: value is inserted into the Sequence according to numerical value (alphabetical for strings)
int Sequence::insert(const ItemType& value)
{
	if (m_size == 0)							//easier to simply reuse code from above for the empty case
	{
		insert(0, value);
		return 0;								//inserted @ position 0 always
	}
	int pos = 0;								//keeps track of position to return
	Node* p = m_head;							//keeps track of position within current sequence
	while (p != nullptr && p->value < value)	//iterate until value at position is no longer < given value
	{
		p = p->next;
		pos++;
	}
	if (p == nullptr)							//if p==nullptr is true, then position is at the end
		insert(m_size, value);
	else if (pos == 0)
		insert(0, value);						//if p is the first pointer, then insert at beginning
	else
		insert(pos, value);						//negligible difference in runtime from reusing code without iteration, easier to reuse code
	return pos;									//return position of insertion
}

//POSTCONDITION: if pos is within bounds, item at pos in the Sequence is removed and deleted
bool Sequence::erase(int pos)
{
	if (pos < 0 || pos >= m_size)				//boundary check
		return false;
	m_size--;									//decrement size (doing so now saves a quarter second to type a "- 1", but adds more time to type this)
	Node*p;										//will point to Node to be marked for deletion

	if (pos <= m_size - pos)					//determine fastest direction to reach position
		p = m_head;
	else
		p = m_tail;

	if (m_size == 0)							//if the Sequence is set to be empty (has one item), set it to empty
	{											//p will be dealt with at the end of the program
		m_head = nullptr;
		m_tail = nullptr;
	}
	else if (pos == 0)							//if Sequence is at least 2 items long, and pos is first position
	{
		m_head = m_head->next;					//only needs to shift m_head to the second item in the Sequence
		m_head->back = nullptr;					//p currently points to the old m_head due to the first if-statement, will be deleted
	}
	else if (pos == m_size)						//if pos is at the last item of the Sequence (uses m_size because it has been decremented)
	{
		m_tail = m_tail->back;					//shifts m_tail to the second-to-last item in the Sequence
		m_tail->next = nullptr;					//p currently points to the old m_tail
	}
	else										//otherwise, needs to iterate through to given position
	{
		if (p == m_head)
			for (int i = 0; i < pos; i++)
				p = p->next;					//iteration forwards from m_head
		else
			for (int i = m_size; i > pos; i--)	//again, m_size is not - 1, already decremented at the start
				p = p->back;					//iteration backwards from m_tail
		p->back->next = p->next;				//eliminate p from the Node map by having no other Nodes from this Sequence point to it
		p->next->back = p->back;
	}
	delete p;									//delete p
	return true;
}

//POSTCONDITION: all Items with the given value are erased from the Sequence
int Sequence::remove(const ItemType& value)
{
	int removed = 0;							//track number of removed items
	int pos = 0;								//tracks current numerical position (used to be able to reuse erase(int))
	Node* p = m_tail;							//starts from m_tail to avoid silly increment/decrement stuff
	for (pos = m_size - 1; p != nullptr && pos >= 0; pos--)		//for loop instead of while loop for preference, decrementing occurs in the same way
	{
		if (p->value == value)					//uses no additional Node pointer, instead uses pos to keep track of deletion positions
		{										//and p to keep track of location
			p = p->back;						//p is set to the previous node (placed in here to allow value access in if-statement)
			erase(pos);							//erase node
			removed++;							//increment removal count
		}
		else
			p = p->back;						//p is set to the previous node, looks ugly and redundant but isn't redundant at all, and works
	}
	return removed;								//return with removal count
}

//POSTCONDITION: if pos is within bounds, set value equal to the current value at pos in the Sequence
bool Sequence::get(int pos, ItemType& value) const
{
	if (pos < 0 || pos >= m_size)				//boundary check
		return false;
	Node* p;									//marker Node pointer
	if (pos <= m_size - pos)					//same as in previous instances of pos, find closest m_tail or m_head
	{
		p = m_head;								//start from the head
		for (int i = 0; i < pos; i++)			//iterate forwards until position is reached
			p = p->next;
	}
	else
	{
		p = m_tail;								//start from tail
		for (int i = m_size - 1; i > pos; i--)	//iterate backwards until position is reached
			p = p->back;
	}
	value = p->value;							//set value to the value at p
	return true;								//return with true
}

//POSTCONDITION: if pos is within bounds, the value at pos in the Sequence is set to the given value
bool Sequence::set(int pos, const ItemType& value)
{
	if (pos < 0 || pos >= m_size)				//boundary check
		return false;
	Node* p;									//marker Node pointer
	if (pos <= m_size - pos)					//same as in previous instances of pos, find closest m_tail or m_head
	{
		p = m_head;								//start from the head
		for (int i = 0; i < pos; i++)			//iterate forwards until position is reached
			p = p->next;
	}
	else
	{
		p = m_tail;								//start from tail
		for (int i = m_size - 1; i > pos; i--)	//iterate backwards until position is reached
			p = p->back;
	}
	p->value = value;							//set value at p to the value given
	return true;								//return with true
}

//returns the first index at which a value is found. If not found, returns -1
int Sequence::find(const ItemType& value) const
{
	int pos = 0;								//tracks current numerical position
	Node* p = m_head;							//starts from m_head to track actual position
	while (p != nullptr)						//iterate until reaches end
	{
		if (p->value == value)					//if current value is at current position
			return pos;							//return with position
		p = p->next;							//move to next pointer
		pos++;									//increment position
	}
	return -1;									//loop has finished without returning, not found -> return w/ -1
}

//POSTCONDITION: this Sequence and a given other Sequence have all values swapped
void Sequence::swap(Sequence& other)
{
	Sequence temp(other);						//create temporary Sequence, copy constructor with other
	other = *this;								//assignment operator= other to this Sequence
	*this = temp;								//set this to the temp, which is the old other
}

//returns the first position in seq1 that starts a subsequence that matches seq2 exactly, -1 if not found
int subsequence(const Sequence& seq1, const Sequence& seq2)
{
	if (seq2.size() == 0 || seq2.size() > seq1.size())
		return -1;								//preliminary bounds check
	ItemType s1, s2;							//needs ItemType to use Sequence::get(int,ItemType&) with since this is a member function
	for (int i = 0; i < seq1.size(); i++)		//iterate through seq1 to find seq2
	{
		int index = 0;							//represents the index within seq2
		seq1.get(i, s1);						//set s1 to the current index in seq1
		seq2.get(0, s2);						//set s2 to the first of seq2
		for (index = 0; s1 == s2 && index < seq2.size(); index++)
		{										//the above for loop first checks that s1 == s2 before beginning, and does not continue without those conditions
			if (!seq1.get(index + i, s1))		//if out of bounds (index + i is outside of seq1 bounds, no more to iterate), return -1, not found
				return -1;
			seq2.get(index, s2);				//retrieve seq2's next item
		}
		if (s1 == s2 && index == seq2.size())	//for loop has finished, so either s1 != s2 or seq2 has been reached
			return i;							//if s1 still is equal to s2, and index has reached the end of seq2, then return current pos in seq1
	}
	return -1;									//default return
}

//POSTCONDITION: result is the resulting combination of alternates from seq1 and seq2
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
	Sequence r;									//create a new Sequence to store results (seq1 and seq2 if they are the same as result)
	ItemType s;									//needed for retrieval and insertions with get(int,ItemType&) and set(int,ItemType&)
	if (seq2.size() > seq1.size())				//if seq2 is larger than seq1
	{
		r = Sequence(seq2);						//seq2 is "laid out" for insertion of seq1
		for (int ins = 0, ind = 0; ind < seq1.size(); ind++, ins += 2)
		{										//increment every two in r, every 1 in seq1
			seq1.get(ind, s);					//retrieve current index from seq1
			r.insert(ins, s);					//insert retrieved ItemType into every other index in r starting from 0
		}
	}
	else
	{											//seq1 is >= seq2 in size
		r = Sequence(seq1);						//seq1 is "laid out" for insertion of seq2
		for (int ins = 1, ind = 0; ind < seq2.size(); ind++, ins += 2)
		{										//increment every two in r, every 1 in seq2
			seq2.get(ind, s);					//retrieve current index from seq2
			r.insert(ins, s);					//insert retrieved ItemType into every other index in r starting from 1
		}
	}
	result = r;									//set result to r (done at the end in case any arguments match the third)
}

//POSTCONDITION: All items in the Sequence are deleted, m_head and m_tail set to nullptr, m_size = 0, emptied
//PRIVATE FUNCTION
void Sequence::deleteList()
{
	Node* p = m_head;
	while (p != nullptr)
	{
		Node* delet_dis = p;
		p = p->next;
		delete delet_dis;
	}
	m_head = nullptr;
	m_tail = nullptr;
	m_size = 0;
}