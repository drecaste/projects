#ifndef BSTSET_HPP
#define BSTSET_HPP

#include "Set.hpp"


template <typename T>
class BSTSet : public Set<T>
{
public:
    BSTSet();

    virtual ~BSTSet();

    BSTSet(const BSTSet& s);

    BSTSet& operator=(const BSTSet& s);

    virtual void add(const T& element);

    virtual bool contains(const T& element) const;

    virtual unsigned int size() const;


private:
	struct Node
	{
		T key;
		Node* left = nullptr;
		Node* right = nullptr;
	};
	
	Node* head;
	int Size;
	
	void destroyBST(Node* node);
  void BSTSet<T>::copyBST(Node* oldN, Node* newN);
};


template <typename T>
BSTSet<T>::BSTSet()
{
	head = nullptr;
	Size = 0;
}


template <typename T>
BSTSet<T>::~BSTSet()
{
    destroyBST(head);
}


template <typename T>
BSTSet<T>::BSTSet(const BSTSet& s)
	: head{nullptr}
{
    copyBST(s.head, head);
}


template <typename T>
BSTSet<T>& BSTSet<T>::operator=(const BSTSet& s)
{
	if(this != &s)
	{
		destroyBST(head);
		head = nullptr;
		copyBST(s.head, head);
	}
    return *this;
}


template <typename T>
void BSTSet<T>::add(const T& element)
{
	if(head == nullptr)
	{
		head = new Node{element};
	}
	else
	{
		Node* current = head;

		while(current != nullptr)
		{
			if(current->key == element)
			{
				return;
			}
			if(element < current->key)
			{
				if(current->left != nullptr)
				{
					current = current->left;
				}
				else
				{
					Node* newNode = new Node;
					newNode->key = element;
					current->left = newNode;
                    Size++;
				}
			}
			if(element > current->key)
			{
				if(current->right != nullptr)
				{
					current = current->right;
				}
				else
				{
					Node* newNode = new Node;
					newNode->key = element;
					current->right = newNode;
                    Size++;
				}
			}
		}
	}
}


template <typename T>
bool BSTSet<T>::contains(const T& element) const
{
	Node* current = head;
	
	while(current != nullptr)
	{
		if(current->key == element)
		{
			return true;
		}
		else
		{
			if(element < current->key)
			{
				current = current->left;
			}
			else
			{
				current = current->right;
			}
		}
	}
    return false;
}


template <typename T>
unsigned int BSTSet<T>::size() const
{
    return Size;
}


template <typename T>
void BSTSet<T>::destroyBST(Node* node)
{
	if(node != nullptr)
	{
		destroyBST(node->left);
		destroyBST(node->right);
		delete node;
	}
}


template <typename T>
void BSTSet<T>::copyBST(Node* a, Node* b)
{
	if(a != nullptr)
	{
		Node* b = new Node;
		b->key = a->key;
		copyBST(b->left, a->left);
		copyBST(b->right, a->right);
		Size++;
	}
}

#endif // BSTSET_HPP
