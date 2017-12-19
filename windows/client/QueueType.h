#pragma once
#include <iostream>
#include <string>

using namespace std;

#define MAXQueue 50
class FullQueue
{
public:
	/**
	*	@brief	Display a message for full queue on screen.
	*/
	void print()
	{
		cout << "FullQueue exception thrown." << endl;
	}
};


/**
*	@brief	Exception class thrown by Dequeue when queue is empty.
*/
class EmptyQueue
{
public:
	/**
	*	@brief	Display a message for empty queue on screen.
	*/
	void print()
	{
		cout << "EmtpyQueue exception thrown." << endl;
	}
};

template <class T>
class QueueType {
private:
	T* items;
	int front;
	int rear;
	int maxQue;
public:
	QueueType(); 
	QueueType(int max); // PARAMETERIZED CONSTRUCTOR 
	~QueueType( ) ; // DESTRUCTOR .
	bool IsFull( ) const; 
	bool IsEmpty();
	void Enqueue( T item ); 
	void Dequeue( T&  item );
	
};
template<class T>
QueueType<T>::QueueType()
{
	maxQue = MAXQueue + 1;
	front = maxQue - 1;
	rear = maxQue - 1;
	items = new T[maxQue];
}
template<class T> 
QueueType<T>::QueueType(int max)  // PARAMETERIZED 
{ 
	maxQue = max + 1; 
	front = maxQue - 1; 
	rear = maxQue - 1; 
	items = new T[maxQue];   // dynamically allocates 
}
template<class T> 
bool QueueType<T>::IsEmpty() 
{ 
	return (rear == front);
}

template<class T> 
QueueType<T>::~QueueType() {
	delete [] items; // deallocates array
}

template<class T> 
bool QueueType<T>::IsFull() const
{ // WRAP AROUND 
	return ((rear + 1) % maxQue == front);
}

template<class T> 
void QueueType<T>::Enqueue(T newItem) 
{ 
	if (IsFull()) throw FullQueue(); 
	else 
	{ 
		rear = (rear + 1) % maxQue;
		items[rear] = newItem; 
	} 
}
template<class T> 
void QueueType<T>::Dequeue(T &item)
{ // WRAP AROUND 
	if (IsEmpty()) throw EmptyQueue();
	else
	{
		front = (front + 1) % maxQue; 
		item = items[front];
	}
}

