/*
	CS32 PROJECT 2
	Christian Yu
	104785497
	CS32-2 w/ Smallberg
*/

#ifndef SEQUENCE_H			//guard
#define SEQUENCE_H

#define USE_STRING			//comment out for using other ItemType

#ifdef USE_STRING
#include <string>			//included only when using std::string as ItemType
typedef std::string ItemType;
#else
typedef unsigned long ItemType;	//defaulted to std::string
#endif

class Sequence
{
public:
	Sequence();										//default constructor
	Sequence(const Sequence& old);					//copy constructor
	Sequence& operator=(const Sequence& old);		//= operator
	~Sequence();									//destructor
	bool empty() const;								//checks if Sequence is empty
	int size() const;								//returns current size of Sequence
	bool insert(int pos, const ItemType& value);	//inserts value at position
	int insert(const ItemType& value);				//inserts value based on relative numerical value (alphabetical)
	bool erase(int pos);							//removes item at position
	int remove(const ItemType& value);				//removes all items with the same as the given value
	bool get(int pos, ItemType& value) const;		//sets value to the item at position
	bool set(int pos, const ItemType& value);		//sets item at position to the value
	int find(const ItemType& value) const;			//returns the first position at which value is found, -1 otherwise
	void swap(Sequence& other);						//swaps the values with another Sequence

private:
	struct Node										//Nodes for doubly-linked list
	{
		Node* next;									//address of the next item
		Node* back;									//address of the previous item
		ItemType value;								//value of this node
	};
	Node* m_head;									//denotes start
	Node* m_tail;									//denotes end
	int m_size;										//current size

	void deleteList();								//removes all nodes from memory, sets to empty-case
};

int subsequence(const Sequence& seq1, const Sequence& seq2);	//finds position of subsequence
void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result);		//result is set to interleave of seq1 and seq2

#endif