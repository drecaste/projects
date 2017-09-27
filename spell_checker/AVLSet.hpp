#ifndef AVLSET_HPP
#define AVLSET_HPP

#include "Set.hpp"


template <typename T>
class AVLSet : public Set<T>
{
public:
    AVLSet();

    virtual ~AVLSet();

    AVLSet(const AVLSet& s);

    AVLSet& operator=(const AVLSet& s);

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
	
	void destroyAVL(Node* node);
	void copyAVL(Node* a, Node* b);
	struct Node* rightRotation(Node* node);
	struct Node* leftRotation(Node* node);
	struct Node* balanceTree(Node* node, const T& element);
	struct Node* insertElement(Node* node, const T& element);
	int Height(Node* node);
	int heightDiff(Node* node);
};


template <typename T>
AVLSet<T>::AVLSet()
{
	head = nullptr;
	Size = 0;
}


template <typename T>
AVLSet<T>::~AVLSet()
{
	destroyAVL(head);
}


template <typename T>
AVLSet<T>::AVLSet(const AVLSet& s)
	: head{nullptr}
{
	copyAVL(s.head, head);
}


template <typename T>
AVLSet<T>& AVLSet<T>::operator=(const AVLSet& s)
{
	if(this != &s)
	{
		destroyAVL(head);
		head = nullptr;
		copyAVL(s.head, head);
	}
    return *this;
}


template <typename T>
struct AVLSet<T>::Node* AVLSet<T>::rightRotation(Node* node)
{
	Node* newRoot;
	newRoot = node->left;	      	// newRoot = B left (which is A)
	node->left = newRoot->right;	// B left = A right (T2)
	newRoot->right = node;		    // A right = B
	
	return newRoot;
}


template <typename T>
struct AVLSet<T>::Node* AVLSet<T>::leftRotation(Node* node)
{
	Node* newRoot;
	newRoot = node->right;	    	// newRoot = A right (which is B)
	node->right = newRoot->left;	// A right = B left (T2)
	newRoot->left = node;		      // B left = A

	return newRoot;
}

template <typename T>
void AVLSet<T>::add(const T& element)
{
	head = insertElement(head, element);
	Size++;
	return;
}


template <typename T>
bool AVLSet<T>::contains(const T& element) const
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
struct AVLSet<T>::Node* AVLSet<T>::insertElement(Node* node, const T& element)
{
	if(node == nullptr)
	{
		Node* node = new Node;
		node->key = element;
		return node;
	}
	if(element == node->key)
	{
		return node;
	}
	if(element < node->key)
	{
		node->left = insertElement(node->left, element);
	}

	if(element > node->key)
	{
		node->right = insertElement(node->right, element);
	}
	return balanceTree(node, element);
}



template <typename T>
struct AVLSet<T>::Node* AVLSet<T>::balanceTree(Node* node, const T& element)
{
	// if heightDiff < - 1	: link is to the right
	// if heightDiff > 1	: link is to the left
	// if element > node	: link is to the right
	// if element < node  	: link is to the left

	int diff = heightDiff(node);

	// RIGHT RIGHT ROTATION
	if((diff < -1) && (element > node->right->key))
	{
		node = leftRotation(node);
	}

	// LEFT LEFT ROTATION
	if((diff > 1) && (element < node->left->key))
	{
		node = rightRotation(node); //current =
	}

	// RIGHT LEFT ROTATION
	if((diff < -1) && (element < node->right->key))
	{
		node->right = rightRotation(node->right);
		node = leftRotation(node);
	}

	// LEFT RIGHT ROTATION
	if((diff > 1) && (element > node->left->key))
	{
		node->left = leftRotation(node->left);
		node = rightRotation(node);
	}
	return node;
}


template <typename T>
int AVLSet<T>::Height(Node* node)
{
	if(node != nullptr)
	{
		return std::max(Height(node->left), Height(node->right)) + 1;
	}
	return 0;
}

template <typename T>
int AVLSet<T>::heightDiff(Node* node)
{
	return Height(node->left) - Height(node->right);
}


template <typename T>
unsigned int AVLSet<T>::size() const
{
    return Size;
}


template <typename T>
void AVLSet<T>::destroyAVL(Node* node)
{
	if(node != nullptr)
	{
		destroyAVL(node->left);
		destroyAVL(node->right);
		delete node;
	}
}


template <typename T>
void AVLSet<T>::copyAVL(Node* a, Node* b)
{
	if(a != nullptr)
	{
		Node* b = new Node;
		b->key = a->key;
		b->height = a->height;
		copyAVL(b->left, a->left);
		copyAVL(b->right, a->right);
		Size++;
	}
}


#endif // AVLSET_HPP
