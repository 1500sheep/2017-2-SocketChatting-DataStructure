#pragma once
#include <iostream>
using namespace std;

#define maxQueue 10

/**
*	@brief	Exception class thrown by Push when queue is full.
*/
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
*	@brief	Exception class thrown by Dequeue and Rear when queue is empty.
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

/**
*	@brief	Queue class.
*	@details	This class processes as First In, First Out (FIFO).
*/
template <typename T>
class Queue
{
public:
	/**
	*	@brief	Allocate dynamic array whose size is maxQueue.
	*	@pre	The maxQueue is defined
	*	@post	Member of items points the allocated array.
	*/
	Queue();

	/**
	*	@brief	Allocate dynamic array whose size is max.
	*	@pre	The max is defined
	*	@post	Member of items points the allocated array.
	*/
	Queue(int max);

	/**
	*	@brief	Destruct the object. Free the array dynamically allocated.
	*	@pre	Release memory for queue pointer.
	*	@post	none.
	*/
	~Queue();


	/**
	*	@brief	Determines whether the queue is full.
	*	@pre	Queue has been initialized.
	*	@post	Function value = (queue is full)
	*/
	bool IsFull();

	/**
	*	@brief	Determines whether the queue is empty.
	*	@pre	Queue has been initialized.
	*	@post	Function value = (queue is empty)
	*/
	bool IsEmpty();

	/**
	*	@brief	Makes queue empty.
	*	@pre	Queue has some records or not.
	*	@post	Queue hasn't any records.
	*/
	void MakeEmpty();

	/**
	*	@brief	Adds newItem to the front of the queue.
	*	@pre	Queue has been initialized.
	*	@post	If (queue is full), FullQueue exception is thrown; otherwise, newItem is at the front of the queue.
	*/
	void EnQueue(T item);

	/**
	*	@brief	Removes front item from the queue.
	*	@pre	Queue has been initialized.
	*	@post	If (queue is empty), EmptyQueue exception is thrown; otherwise, front element has been removed from queue.
	*/
	void DeQueue(T& item);

	/**
	*	@brief	Print all the items in the queue on screen
	*	@pre	Queue has been initialized.
	*	@post	None.
	*/
	void Print();

private:
	int m_iFront;	///< Number of front of queue.
	int m_iRear;	///< Number of Rear of queue
	int m_nMaxQueue;///< Size of queue
	T *m_pItems;	///< Pointer for a queue.
};


// Allocate dynamic array whose size is maxQueue 10.
template <typename T>
Queue<T>::Queue()
{
	m_iFront = (maxQueue - 1) % maxQueue;
	m_iRear = (maxQueue - 1) % maxQueue;
	m_nMaxQueue = maxQueue;
	m_pItems = new T[maxQueue];

}

// Allocate dynamic array whose size is max
template <typename T>
Queue<T>::Queue(int max)
{
	m_iFront = (max - 1) % max;
	m_iRear = (max - 1) % max;
	m_nMaxQueue = max;
	m_pItems = new T[max];
}

// Destruct the object. Free the array dynamically allocated.
template <typename T>
Queue<T>::~Queue()
{
	delete[] m_pItems;
}

// Determines whether the queue is full.
template <typename T>
bool Queue<T>::IsFull()
{
	if ((m_iRear + 1) % maxQueue == m_iFront)
		return true;
	else
		return false;
}

// Determines whether the queue is empty.
template <typename T>
bool Queue<T>::IsEmpty()
{
	if (m_iFront == m_iRear)
		return true;
	else
		return false;
}

//	Removes All items in queue records.
template <typename T>
void Queue<T>::MakeEmpty()
{
	m_iRear = m_iFront;
}

//	Add item in queue records.
template <typename T>
void Queue<T>::EnQueue(T item)
{
	if (IsFull())			//	Queue�� Full ������ ��
		throw FullQueue();	//	Full���¿� ���� ��½���
	else
	{
		m_iRear++;							//	rear��ġ�� ����
		if (m_iRear == m_nMaxQueue)			//	Circular�� ���� �� rear���� �� ���� ������ ���
		{
			m_iRear = m_iRear % m_nMaxQueue;//	rear��ġ�� ������

		}
		m_pItems[m_iRear] = item;			//	���ڵ忡 item�� ����
	}
}

//	Delete item in queue records.
template <typename T>
void Queue<T>::DeQueue(T& item)
{
	if (IsEmpty())			//	Queue�� Empty ������ ��
		throw EmptyQueue();	//	Empty���¿� ���� ��½���
	else
	{
		m_iFront = (m_iFront + 1) % m_nMaxQueue;//	front��ġ�� ����
		item = m_pItems[m_iFront];				//	��¿��� ������ �������� ����
	}
}

// Print all the items in the queue on screen
template <typename T>
void Queue<T>::Print()
{
	cout << "Queue: ";
	// Queue ���� ��� element ���.
	int i = m_iFront;		//	��½�ų ������ ����
	while (i != m_iRear)	//	rear��ġ������ ���
	{
		i = (i + 1) % m_nMaxQueue;		//	�������� ��ġ���� �� ĭ�� ����
		cout << m_pItems[i] << " - ";	//	���ڵ尪 ���
	}
	cout << endl;
}