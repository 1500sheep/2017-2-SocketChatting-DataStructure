#pragma once
#include <iostream>
using namespace std;

#define maxStack 10


/**
*	@brief	Exception class thrown by Push when stack is full.
*/
class FullStack
{
public:
	/**
	*	@brief	Display a message for full stack on screen.
	*/
	void print()
	{
		cout << "FullStack exception thrown." << endl;
	}
};


/**
*	@brief	Exception class thrown by Pop and Top when stack is empty.
*/
class EmptyStack
{
public:
	/**
	*	@brief	Display a message for full stack on screen.
	*/
	void print()
	{
		cout << "EmtpyStack exception thrown." << endl;
	}
};


/**
*	@brief	Stack class.
*	@details	This class processes as Last In, First Out (LIFO).
*/
template <typename T>
class Stack
{
public:

	/**
	*	@brief	Allocate dynamic array whose size is maxStack.
	*	@pre	The maxStack is defined
	*	@post	Member of items points the allocated array.
	*/
	Stack();

	/**
	*	@brief	Destruct the object. Free the array dynamically allocated.
	*	@pre	Release memory for stack pointer.
	*	@post	none.
	*/
	~Stack();

	/**
	*	@brief	Determines whether the stack is full.
	*	@pre	Stack has been initialized.
	*	@post	Function value = (stack is full)
	*/
	bool IsFull() const;

	/**
	*	@brief	Determines whether the stack is empty.
	*	@pre	Stack has been initialized.
	*	@post	Function value = (stack is empty)
	*/
	bool IsEmpty() const;

	/**
	*	@brief	Adds newItem to the top of the stack.
	*	@pre	Stack has been initialized.
	*	@post	If (stack is full), FullStack exception is thrown; otherwise, newItem is at the top of the stack.
	*/
	void Push(T item);

	/**
	*	@brief	Removes top item from the stack.
	*	@pre	Stack has been initialized.
	*	@post	If (stack is empty), EmptyStack exception is thrown; otherwise, top element has been removed from stack.
	*/
	T Pop(T& item);

	/**
	*	@brief	Returns a copy of top item on the stack.
	*	@pre	Stack has been initialized.
	*	@post	If (stack is empty), EmptyStack exception is thrown; otherwise, top element has been removed from stack.
	*/
	T Top();

	/**
	*	@brief	Print all the items in the stack on screen
	*	@pre	Stack has been initialized.
	*	@post	None.
	*/
	void Print();

private:
	int top;	///< Number of elements.
	T *items;	///< Pointer for a stack.
};

// Allocate dynamic array whose size is maxStack 10.
template <typename T>
Stack<T>::Stack()
{
	items = new T[maxStack];
	top = -1;
}

// Destruct the object. Free the array dynamically allocated.
template <typename T>
Stack<T>::~Stack()
{
	delete[] items;
}

// Determines whether the stack is empty.
template <typename T>
bool Stack<T>::IsEmpty() const
{
	if (top <= -1)
		return true;
	else
		return false;
}

// Determines whether the stack is full.
template <typename T>
bool Stack<T>::IsFull() const
{
	if (top >= maxStack - 1)
		return true;
	else
		return false;
}

//	Add item in stack records.
template <typename T>
void Stack<T>::Push(T newItem)
{
	if (IsFull())
		throw FullStack();
	else
	{
		top++;
		items[top] = newItem;
	}
}

//	Delete item in stack records.
template <typename T>
T Stack<T>::Pop(T& item)
{
	T temp;
	if (IsEmpty())
		throw EmptyStack();
	else
	{
		item = items[top];
		top--;
		return item;
	}
}

// Determines the top item in stack.
template <typename T>
T Stack<T>::Top()
{
	if (IsEmpty())
		throw EmptyStack();
	else
		return items[top];
}

// Print all the items in the stack on screen
template <typename T>
void Stack<T>::Print()
{
	cout << "Stack: ";
	for (int i = top; i >= 0; i--)
		cout << items[i] << " - ";
	cout << endl;
}