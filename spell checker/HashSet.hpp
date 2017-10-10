#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"


template <typename T>
class HashSet : public Set<T>
{
public:
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    typedef std::function<unsigned int(const T&)> HashFunction;

public:
    HashSet(HashFunction hashFunction);

    virtual ~HashSet();

    HashSet(const HashSet& s);

    HashSet& operator=(const HashSet& s);

    virtual void add(const T& element);

    virtual bool contains(const T& element) const;

    virtual unsigned int size() const;


private:
    HashFunction hashFunction;
    
	struct Node
	{
		T value;
		Node* next = nullptr;
	};
	
	Node** keytable = nullptr;
	int Capacity;
	int Size;
	
	void setnull();
	void HSinsert(const T& element);
	void delHashSet(Node** kt, int cap);
	void copy(Node** k, int cap);
};


template <typename T>
void HashSet<T>::setnull()
{
	for(int i = 0; i < Capacity; i++)
	{
		keytable[i] = nullptr;
	}
}


template <typename T>
HashSet<T>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
	keytable = new Node*[DEFAULT_CAPACITY];
	Capacity = DEFAULT_CAPACITY;
	Size = 0;
	setnull();
}

template <typename T>
void HashSet<T>::delHashSet(Node** kt, int cap)
{
	for(int i = 0; i < cap; i++)
	{
		while(kt[i] != nullptr)
		{
			Node* deleting = kt[i];
			kt[i] = kt[i]->next;
			delete deleting;
		}
	}
	delete[] kt;
	kt = nullptr;
}

template <typename T>
HashSet<T>::~HashSet()
{
	delHashSet(keytable, Capacity);
}


template <typename T>
HashSet<T>::HashSet(const HashSet& s)
    : hashFunction{s.hashFunction}
{
	keytable = new Node*[DEFAULT_CAPACITY];
	Capacity = s.Capacity;
	Size = s.Size;
	setnull();
	copy(s.keytable, s.Capacity);
}


template <typename T>
void HashSet<T>::copy(Node** k, int cap)
{
	for(int i = 0; i < cap; i++)
	{
		if(k[i] != nullptr)
		{
			while(k[i] != nullptr)
			{
				add(k[i]->value);
				k[i] = k[i]->next;
			}
		}
	}
}


template <typename T>
HashSet<T>& HashSet<T>::operator=(const HashSet& s)
{
    if (this != &s)
    {
        hashFunction = s.hashFunction;
		copy(s.keytable, s.Capacity);
    }
    return *this;
}


template <typename T>
void HashSet<T>::add(const T& element)
{
	if((Size / Capacity) < 0.8)
	{
		HSinsert(element);
	}
	else
	{
		Node** temp_keytable = keytable;
		Capacity = Capacity * 2;
		keytable = new Node*[Capacity];
		setnull();
		for(int i = 0; i < Capacity / 2; i++)
		{
			if(temp_keytable[i] != nullptr)
			{
				while(temp_keytable[i] != nullptr)
				{
					HSinsert(temp_keytable[i]->value);
					temp_keytable[i] = temp_keytable[i]->next;
				}
			}
		}
		HSinsert(element);
		delHashSet(temp_keytable, Capacity / 2);
	}
}


template<typename T>
void HashSet<T>::HSinsert(const T& element)
{
	int keytable_value = hashFunction(element) % Capacity;

	Node* newNode = keytable[keytable_value];

	Node* nodeWithNewValue = new Node;
	nodeWithNewValue->value = element;
	
	nodeWithNewValue->next = newNode;
	keytable[keytable_value] = nodeWithNewValue;

	Size++;
}


template <typename T>
bool HashSet<T>::contains(const T& element) const
{
	int keytable_value = hashFunction(element) % Capacity;

	Node* newNode = keytable[keytable_value];
	
	while(newNode != nullptr)
	{
		if(element == newNode->value)
		{
			return true;
		}
		else
		{
			newNode = newNode->next;
		}
	}
   	return false;
}


template <typename T>
unsigned int HashSet<T>::size() const
{
    return Size;
}



#endif // HASHSET_HPP
