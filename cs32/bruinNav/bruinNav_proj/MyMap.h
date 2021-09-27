// MyMap.h
#ifndef MYMAP_H
#define MYMAP_H
// Skeleton for the MyMap class template.  You must implement the first six
// member functions.

template<typename KeyType, typename ValueType>
class MyMap
{
public:
	MyMap() { m_bst = nullptr; }		//initialize empty
	~MyMap() { clear(); }				//delete all initialized items

	void clear() { deleteNode(m_bst); }	//run recursive delete function on head Node
	int size() const { return treeSize(m_bst); }	//run recursive size function on head Node

	void associate(const KeyType& key, const ValueType& value)
	{
		if (m_bst == nullptr)
			m_bst = new Node(key, value);
		Node *current = m_bst;
		while (current != nullptr && current->key != key)
		{
			if (current->key > key)					//if the insertion key is less, place left
			{
				if (current->l_child != nullptr)
					current = current->l_child;
				else
				{
					current->l_child = new Node(key, value);
					return;							//no additional action required
				}
			}
			if (current->key < key)					//if the insertion key is greater, place right
			{
				if (current->r_child != nullptr)
					current = current->r_child;
				else
				{
					current->r_child = new Node(key, value);
					return;
				}
			}
		}
		current->value = value;
	}

	// for a map that can't be modified, return a pointer to const ValueType
	const ValueType* find(const KeyType& key) const { return search(key, m_bst); }

	// for a modifiable map, return a pointer to modifiable ValueType
	ValueType* find(const KeyType& key)
	{
		return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
	}



	// C++11 syntax for preventing copying and assignment
	MyMap(const MyMap&) = delete;
	MyMap& operator=(const MyMap&) = delete;

private:
	struct Node
	{
		Node(const KeyType& newKey, const ValueType& newValue)
			:key(newKey), value(newValue), l_child(nullptr), r_child(nullptr) {}

		KeyType key;
		ValueType value;

		Node* l_child;
		Node* r_child;
	};
	Node* m_bst;

	void deleteNode(Node* n)
	{
		if (n == nullptr)
			return;
		deleteNode(n->l_child);
		deleteNode(n->r_child);
		delete n;
	}

	int treeSize(Node* n) const
	{
		if (n == nullptr)
			return 0;
		return treeSize(n->l_child) + treeSize(n->r_child) + 1;
	}

	const ValueType* search(const KeyType& key, Node* tree) const
	{
		if (tree == nullptr)
			return nullptr;
		if (key == tree->key)
			return &(tree->value);
		if (key > tree->key)
			return search(key, tree->r_child);
		return search(key, tree->l_child);
	}
};

#endif