#pragma once
#include "Queue.h"
#include "Stack.h"

#include <iostream>
#include <iomanip>
#include <string>

#define MAXVERTICES 15




/**
*	@brief		Graph class.
*	@details	This class includes vertices and each weight.
*/
template <typename T>
class Graph
{
public:
	/**
	*	default constructor.
	*/
	Graph();

	/**
	*	Constructor.
	*/
	Graph(int maxVertices);

	/**
	*	Destructor.
	*/
	~Graph();

	/**
	*	@brief	Initialize graph to empty state.
	*	@pre	None
	*	@post	Graph is empty.
	*/
	void MakeEmpty();

	/**
	*	@brief	Check capacity of graph is empty.
	*	@pre	none.
	*	@post	none.
	*/
	bool IsEmpty();

	/**
	*	@brief	Check capacity of graph is empty.
	*	@pre	none.
	*	@post	none.
	*/
	bool IsFull();

	/**
	*	@brief	Add vertex into graph.
	*	@pre	Graph is not full. Vertex is not in graph.
	*	@post	Vertex is inserted in this graph.
	*	@param	vertex The vertex which want to add in this graph.
	*	@return	If vertex already in graph, return 2. Graph in full, return 0, else return 1.
	*/
	int AddVertex(T vertex);
	
	/**
	*	@brief	Add weight between two vertices in graph.
	*	@pre	Two vertices should be in the graph.
	*	@post	new weight is added into the edges of two vertices.
	*	@param	fromVertex vertex which is a starting point.
	*	@param	toVertex vertex which is a ending point.
	*	@param	weight The weight which want to add.
	*/
	void AddEdge(T fromVertex, T toVertex, int weight);

	/**
	*	@brief	Get weight of two vertices in the graph.
	*	@pre	Two vertices should be in the graph.
	*	@post	Function value = size of weight.
	*	@param	fromVertex vertex which is a starting point.
	*	@param	toVertex vertex which is a ending point.
	*	@return	Size of weight.
	*/
	int WeightIs(T fromVertex, T toVertex);

	/**
	*	@brief	Get all adjacent vertices of one vertex in the graph, and insert the queue.
	*	@pre	None.
	*	@post	Adjacent vertices inserted in the queue.
	*	@param	vertex the vertex of main.
	*	@param	adgVertices the queue of putting adjacent vertices.
	*/
	void GetToVertices(T vertex, Queue<T>& adgVertices);

	/**
	*	@brief	Clear the mark information.
	*	@pre	None.
	*	@post	the mark information initialize.
	*/
	void ClearMarks();

	/**
	*	@brief	Marking the vertex for escape overlapping to search.
	*	@pre	None.
	*	@post	the vertex is marked.
	*	@param	vertex the vertex being mark.
	*/
	void MarkVertex(T vertex);

	/**
	*	@brief	Check the vertex to marking.
	*	@pre	none.
	*	@post	none.
	*	@param	vertex the vertex wants to check.
	*/
	bool IsMarked(T vertex);

	/**
	*	@brief	Display all vertices in the graph.
	*	@pre	none.
	*	@post	none.
	*/
	void PrintVertices();

	/**
	*	@brief	Display all weights in the graph.
	*	@pre	none.
	*	@post	none.
	*/
	void PrintEdges();

private:
	int numVertices;		/// number of vertex nodes.
	int maxVertices;		/// number of max size of vertex nodes.
	T* vertices;			/// pointer of vertices.
	int** edges;			///	pointer of weighted edges.
	bool* marks;			/// pointer of marks.
};

/**
*	@brief	Return index of vertex.
*	@pre	none.
*	@post	Return index of vertex.
*	@param	vertices array of including vertices.
*	@param	vertex	current vertex.
*	@param	vertexNum number of vertices.
*/
template <class T>
int IndexIs(T* vertices, T vertex, int vertexNum);

/**
*	@brief	Search the path by breadth-first search.
*	@pre	none.
*	@post	Find all possible paths before move to the next level.
*	@param	graph the graph to find the path.
*	@param	startVertex vertex of starting point.
*	@param	endVertex vertex of ending point.
*/
template <class T>
void BreadthFirstSearch(Graph<T>& graph, T startVertex, T endVertex);

/**
*	@brief	Search the path by depth-first search.
*	@pre	none.
*	@post	Find the deep path likes post order traversal.
*	@param	graph the graph to find the path.
*	@param	startVertex vertex of starting point.
*	@param	endVertex vertex of ending point.
*/
template <class T>
void DepthFirstSearch(Graph<T>& graph, T startVertex, T endVertex);

// Default constructor
template<typename T>
Graph<T>::Graph()
{
	numVertices = 0;					
	maxVertices = MAXVERTICES;				
	vertices = new T[maxVertices];			
	edges = new int*[maxVertices];
	for (int i = 0; i < maxVertices; i++)
		edges[i] = new int[maxVertices];
	marks = new bool[maxVertices];
}

// Constructor
template <typename T>
Graph<T>::Graph(int max)
{
	numVertices = 0;
	maxVertices = max;
	vertices = new T[maxVertices];
	edges = new T*[maxVertices];
	for (int i = 0; i < maxVertices; i++)
		edges[i] = new int[maxVertices];
	marks = new bool[maxvertices];
}

// Destructor
template <typename T>
Graph<T>::~Graph()
{
	delete[] vertices;
}

// Make empty the graph
template <typename T>
void Graph<T>::MakeEmpty()
{
	numVertices = 0;
}

// Check the graph is empty
template <typename T>
bool Graph<T>::IsEmpty()
{
	if (numVertices == 0)
		return true;
	else
		return false;
}

// Check the graph is full
template <typename T>
bool Graph<T>::IsFull()
{
	if (numVertices == maxVertices)
		return true;
	else
		return false;
}

// Add new vertex in the graph.
template <typename T>
int Graph<T>::AddVertex(T vertex)
{
	for (int i = 0; i < numVertices; i++)
	{
		if (vertices[i] == vertex)				// 추가하고자 하는 vertex가 이미 존재할 때
			return 2;							// return 값 2 반환
	}

	if (numVertices == maxVertices)				// vertex가 꽉 찼을 때
		return 0;								// return 값 0 반환
	else
	{
		vertices[numVertices] = vertex;			// 현재 위치한 배열에 vertex 저장
		edges[numVertices][numVertices] = 0;	// edge 초기화
		for (int i = 0; i < numVertices; i++)	// 다른 vertex 간 weight를 -1로 초기화
		{
			edges[i][numVertices] = -1;
			edges[numVertices][i] = -1;
		}
		marks[numVertices] = false;				// 현재 배열의 mark 초기화
		numVertices++;							// 전체 vertex 개수 증가
		return 1;								// return 값 1 반환
	}
}

// Add new weight between two vertices in the graph
template<typename T>
void Graph<T>::AddEdge(T fromVertex, T toVertex, int weight)
{
	int from, to;										// edge를 추가할 두 개의 index를 저장하기 위한 int 변수 선언
	from = IndexIs(vertices, fromVertex, numVertices);	// 시작점의 index를 저장
	to = IndexIs(vertices, toVertex, numVertices);		// 종료점의 index를 저장
	if (from == -1 || to == -1)							// 존재하지 않는 vertex에 대한 예외처리
	{
		cout << "\tWrong Vertex" << endl;
		return;
	}
	else
	{
		edges[from][to] = weight;						// 해당 index에 weight값 저장
		cout << "\tSuccessed." << endl;
	}
}

// Get weight of two vertices in the graph.
template<typename T>
int Graph<T>::WeightIs(T fromVertex, T toVertex)
{
	int from, to;										// edge를 반환할 두 개의 index를 저장하기 위한 int 변수 선언

	from = IndexIs(vertices, fromVertex, numVertices);	// 시작점의 index를 저장
	to = IndexIs(vertices, toVertex, numVertices);		// 종료점의 index를 저장
	
	if (from == -1 && to == -1)							// 존재하지 않는 vertex에 대한 예외처리	
		return 0;										// return 값 0 반환
	else
		return edges[from][to];							// 해당 index의 weight값 반환
}

// Get all adjacent vertices of one vertex in the graph, and insert the queue.
template<typename T>
void Graph<T>::GetToVertices(T vertex, Queue<T>& adgVertices)
{
	int verIndex;										// 기준이 되는 vertex의 index를 저장하기 위한 int 변수 선언
	verIndex = IndexIs(vertices, vertex, numVertices);	// 기준 vertex의 index 저장
	for (int i = 0; i < numVertices; i++)				// 존재하는 모든 vertex에 대하여 for문 실행
	{
		if (edges[verIndex][i] != -1)					// 인접한 vertex의 weight값이 -1(초기화값)인 경우
			adgVertices.EnQueue(vertices[i]);			// Queue에 해당 vertex를 추가
	}
}

// Clear the mark information.
template<typename T>
void Graph<T>::ClearMarks()
{
	for (int i = 0; i < numVertices; i++)				// 존재하는 모든 vertex에 대하여 for문 실행
		marks[i] = false;								// mark의 상태를 false로 저장
}

// Marking the vertex for escape overlapping to search.
template<typename T>
void Graph<T>::MarkVertex(T vertex)
{
	int verIndex = IndexIs(vertices, vertex, numVertices);	// Marking할 vertex의 index를 저장
	marks[verIndex] = true;									// mark의 상태를 true로 저장
}

// Check the vertex to marking.
template<typename T>
bool Graph<T>::IsMarked(T vertex)
{
	int verIndex = IndexIs(vertices, vertex, numVertices);	// Marking 여부를 확인할 vertex의 index를 저장

	if (marks[verIndex] == true)							// mark의 상태가 true일 때 true값 반환
		return true;
	else
		return false;
}

// Display all vertices in the graph.
template<typename T>
void Graph<T>::PrintVertices()
{
	for (int i = 0; i < numVertices; i++)					
		cout << setw(10) << vertices[i];					
	cout << endl;
}

// Display all weights in the graph.
template<typename T>
void Graph<T>::PrintEdges()
{
	for (int i = 0; i < numVertices; i++)
	{
		if (i == 0)
			cout << setw(20) << vertices[i];
		else
			cout << setw(10) << vertices[i];
	}
	cout << endl;
	for (int j = 0; j < numVertices; j++)
	{
		cout << setw(10) << vertices[j];
		for (int k = 0; k < numVertices; k++)
		{
			if (edges[j][k] == -1)
				cout << setw(10) << "None";
			else
				cout << setw(10) << edges[j][k];
		}
		cout << endl;
	}
	cout << endl;
}

// Return index of vertex.
template <class T>
int IndexIs(T* vertices, T vertex, int vertexNum)
{
	bool found = false;						// 예외처리를 위한 boolean 변수 선언
	for (int i = 0; i < vertexNum; i++)		
	{
		if (vertices[i] == vertex)			// 일치하는 배열 안의 vertex를 찾았을 경우
			return i;						// 그 vertex의 배열 index값 반환
	}
	if (found == false)						// 일치하는 배열 안의 vertex를 찾지 못했을 경우
		return -1;							// return 값 -1 반환
}

// Search the path by breadth-first search.
template <class T>
void BreadthFirstSearch(Graph<T>& graph, T startVertex, T endVertex)
{
	Queue<T> queue;			// 경로탐색에 필요한 queue 선언
	Queue<T> vertexQueue;	// 인접vertex탐색을 위한 vertexQueue 선언

	bool found = false;		// 탐색여부 확인을 위한 boolean 변수 선언
	T vertex;				// 파라미터 임시 vertex 선언									
	T item;					// 파라미터 임시 item 선언

	graph.ClearMarks();			// Graph 안의 모든 vertex의 mark상태 초기화
	queue.EnQueue(startVertex);	// 경로탐색 queue에 startvertex값 추가

	do {
		queue.DeQueue(vertex);		// queue에서 vertex값을 꺼냄
		if (vertex == endVertex)	// 나온 vertex가 종료점vertex와 같을 경우
		{
			cout << setw(5) << vertex;	// vertex 출력 후 탐색완료
			found = true;
		}
		else
		{
			if (!graph.IsMarked(vertex))					// marking이 안된 vertex일 경우
			{		
				graph.MarkVertex(vertex);					// vertex에 marking
				cout << setw(10) << vertex;					// vertex값 출력
				graph.GetToVertices(vertex, vertexQueue);	// 인접 vertex를 vertexQueue에 저장
				while (!vertexQueue.IsEmpty())				// 인접 vertex가 존재하지 않을 때까지
				{
					vertexQueue.DeQueue(item);				// vertexQueue에서 item을 꺼냄
					if (!graph.IsMarked(item))				// marking이 안된 vertex 일 경우
						queue.EnQueue(item);				// 경로탐색 queue에 item값 추가
				}
			}
		}
	} while (!queue.IsEmpty() && !found);					// 경로가 존재하지 않거나 탐색이 안됐을 때 까지 실행
	if (!found)		// 탐색 실패에 대한 메세지 출력
		cout << "\tPath is not found." << endl;
}

// Search the path by depth-first search.
template <class T>
void DepthFirstSearch(Graph<T>& graph, T startVertex, T endVertex)
{
	Stack<T> stack;					// 경로탐색에 필요한 stack 선언
	Queue<T> vertexQueue;			// 인접vertex탐색을 위한 vertexQueue 선언

	bool found = false;				// 탐색여부 확인을 위한 boolean 변수 선언
	T vertex;						// 파라미터 임시 vertex 선언	
	T item;							// 파라미터 임시 item 선언

	graph.ClearMarks();				// Graph 안의 모든 vertex의 mark상태 초기화
	stack.Push(startVertex);		// 경로탐색 stack에 startvertex값 추가

	do
	{
		stack.Pop(vertex);			// stack에서 vertex값을 꺼냄
		if (vertex == endVertex)	// 나온 vertex가 종료점vertex와 같을 경우
		{
			cout << setw(5) << vertex;	// vertex 출력 후 탐색완료
			found = true;
		}
		else
		{
			if (!graph.IsMarked(vertex))					// marking이 안된 vertex일 경우
			{
				graph.MarkVertex(vertex);					// vertex에 marking
				cout << setw(10) << vertex;					// vertex값 출력
				graph.GetToVertices(vertex, vertexQueue);	// 인접 vertex를 vertexQueue에 저장
				while (!vertexQueue.IsEmpty())				// 인접 vertex가 존재하지 않을 때까지
				{
					vertexQueue.DeQueue(item);				// vertexQueue에서 item을 꺼냄
					if (!graph.IsMarked(item))				// marking이 안된 vertex 일 경우
						stack.Push(item);					// 경로탐색 stack에 item값 추가
				}
			}
		}
	} while (!stack.IsEmpty() && !found);					// 경로가 존재하지 않거나 탐색이 안됐을 때 까지 실행
	if (!found)			// 탐색 실패에 대한 메세지 출력
		cout << "\tPath is not found." << endl;
}
