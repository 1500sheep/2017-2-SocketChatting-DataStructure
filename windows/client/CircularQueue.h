#include <iostream>

using namespace std;

#define MAX_ITEMS 50

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

/**
*	@brief	Queue class.
*	@details	This class processes as First In, First Out (FIFO).
*/
template <class ItemType>
class CircularQueueType  
{
public:
	/**
	*	@brief	MAX_ITEMS max만큼의 사이즈를 가진 큐를 초기화한다.
	*	@pre	The MAX_ITEMS is defined
	*	@post	Member of items points the allocated array.
	*/
	CircularQueueType();

	/**
	*	@brief	max만큼의 사이즈를 가진 큐를 초기화한다.
	*	@pre	The max is defined
	*	@post	Member of items points the allocated array.
	*	@param	큐의 사이즈
	*/
	CircularQueueType(int max);

	/**
	*	@brief	Destruct the object. Free the array dynamically allocated.
	*	@pre	Release memory for stack pointer.
	*	@post	none.
	*/
	~CircularQueueType();

	/**
	*	@brief	Determines whether the queue is full.
	*	@pre	Queue has been initialized.
	*	@post	Function value = (Queue is full)
	*/
	bool IsFull();

	/**
	*	@brief	Determines whether the queue is empty.
	*	@pre	Queue has been initialized.
	*	@post	Function value = (Queue is empty)
	*/
	bool IsEmpty();

	/**
	*	@brief	큐안의 내용을 모두 삭제한다.
	*	@pre	Queue has been initialized.
	*	@post	If (queue is full), FullQueue exception is thrown; otherwise, newItem is at the top of the queue.
	*/
	void MakeEmpty();

	/**
	*	@brief	큐에 새로운 자료를 추가한다.
	*	@pre	Queue has been initialized.
	*	@post	If (queue is full), FullQueue exception is thrown; otherwise, newItem is at the top of the queue.
	*/
	void EnQueue(ItemType newItem);

	/**
	*	@brief	큐의 제일 먼저 들어온 자료부터 출력한다.
	*	@pre	Queue has been initialized.
	*	@post	If (queue is empty), EmptyQueue exception is thrown; otherwise, bottom element has been removed from queue.
	*/
	void DeQueue(ItemType& item);

	/**
	*	@brief	Print all the items in the queue on screen
	*	@pre	Queue has been initialized.
	*	@post	None.
	*/
	void Print();

private:
	int m_iFront;			//큐의 앞을 나타내는 플래그
	int m_iRear;			//큐의 뒤를 나타내는 플래그
	int m_nMaxQueue;		//큐의 사이즈
	ItemType* m_pItems;		// 아이템타입 포인터 변수

};


/***********************************************************************************************************************************************************/

// Allocate dynamic array whose size is MAX_ITEMS 8.
template <class ItemType>
CircularQueueType<ItemType>::CircularQueueType()
{
	m_nMaxQueue = MAX_ITEMS+1;		// 원형 큐의 최대 크기를 MAX_ITEM +1로 선언해서 한칸은 비게한다.
	m_iFront = m_nMaxQueue-1;		// 원형 큐의 선두를 최대크기 -1 로 선언
	m_iRear = m_nMaxQueue-1;			// 원형 큐의 후미를 최대크기 -1 로 선언
	m_pItems = new ItemType[m_nMaxQueue];  
}

// 큐의 사이즈를 받아온 만큼 할당해준다.
template <class ItemType>
CircularQueueType<ItemType>::CircularQueueType(int max)
{
	m_nMaxQueue = max+1;			// 원형 큐의 최대 크기를 max로 선언해서 한칸은 비게한다.
	m_iFront =m_nMaxQueue-1;			// 원형 큐의 선두를 최대크기 -1 로 선언
	m_iRear = m_nMaxQueue-1;			// 원형 큐의 후미를 최대크기 -1 로 선언
	m_pItems = new ItemType[m_nMaxQueue];   
}

// Destruct the object. Free the array dynamically allocated.
template <class ItemType>
CircularQueueType<ItemType>::~CircularQueueType()
{
	delete [] m_pItems;
}

// Determines whether the queue is full.
template <class ItemType>
bool CircularQueueType<ItemType>::IsFull()
{
	if ((m_iRear + 1) % m_nMaxQueue == m_iFront)  // 선두와 후미의 위치를 비교하여 큐가 꽉차있는지 확인. 후미가 선두를 교차하면 안됨.
		return true;
	else
		return false;
}

// Determines whether the queue is empty.
template <class ItemType>
bool CircularQueueType<ItemType>::IsEmpty()
{
	if (m_iFront == m_iRear) // 선두와 후미의 위치를 비교하여 큐가 비어있는지 확인
		return true;
	else
		return false;
}

//큐를 비워준다.
template <class ItemType>
void CircularQueueType<ItemType>::MakeEmpty()
{
	m_iFront = m_iRear;				// 선두와 후미의 위치를 같게 하여 큐를 비움
}

// Adds newItem to the top of the queue.
template <class ItemType>
void CircularQueueType<ItemType>::EnQueue(ItemType newItem)
{
	if(IsFull())
	{
		throw FullQueue();
	}
	else
	{
		m_iRear = (m_iRear + 1) % m_nMaxQueue;		// 큐가 꽉차지 않았다면 후미의 위치를 한단계 옮기고
		m_pItems[m_iRear] = newItem;				// 옮긴 위치에 새로운 아이템 배열을 추가
	}
}

// Removes top item from the queue.
template <class ItemType>
void CircularQueueType<ItemType>::DeQueue(ItemType& item)
{
	if(IsEmpty())
	{
		throw EmptyQueue();
	}
	else
	{
		m_iFront = (m_iFront+1) % m_nMaxQueue;		// 큐가 비어있지 않다면 선두의 위치를 한단계 옮기고
		item = m_pItems[m_iFront];					// 옮긴 위치의 아이템 배열을 삭제
	}
}

// Print all the items in the queue on screen
template <class ItemType>
void CircularQueueType<ItemType>::Print()
{
	cout << "Queue: ";
	// Stack 내의 모든 element 출력.
	for (int i = (m_iFront + 1) % m_nMaxQueue; i != (m_iRear + 1) % m_nMaxQueue; i = (i + 1) % m_nMaxQueue) //큐의 처음+1에서 출발해서 큐의 마지막까지 확인한다. 모든 검색시 mod로 순환하게 한다.
	{
		cout << m_pItems[i] << " - ";
	}
	cout << endl;
}