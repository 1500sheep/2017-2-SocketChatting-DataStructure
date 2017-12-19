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
		if (vertices[i] == vertex)				// �߰��ϰ��� �ϴ� vertex�� �̹� ������ ��
			return 2;							// return �� 2 ��ȯ
	}

	if (numVertices == maxVertices)				// vertex�� �� á�� ��
		return 0;								// return �� 0 ��ȯ
	else
	{
		vertices[numVertices] = vertex;			// ���� ��ġ�� �迭�� vertex ����
		edges[numVertices][numVertices] = 0;	// edge �ʱ�ȭ
		for (int i = 0; i < numVertices; i++)	// �ٸ� vertex �� weight�� -1�� �ʱ�ȭ
		{
			edges[i][numVertices] = -1;
			edges[numVertices][i] = -1;
		}
		marks[numVertices] = false;				// ���� �迭�� mark �ʱ�ȭ
		numVertices++;							// ��ü vertex ���� ����
		return 1;								// return �� 1 ��ȯ
	}
}

// Add new weight between two vertices in the graph
template<typename T>
void Graph<T>::AddEdge(T fromVertex, T toVertex, int weight)
{
	int from, to;										// edge�� �߰��� �� ���� index�� �����ϱ� ���� int ���� ����
	from = IndexIs(vertices, fromVertex, numVertices);	// �������� index�� ����
	to = IndexIs(vertices, toVertex, numVertices);		// �������� index�� ����
	if (from == -1 || to == -1)							// �������� �ʴ� vertex�� ���� ����ó��
	{
		cout << "\tWrong Vertex" << endl;
		return;
	}
	else
	{
		edges[from][to] = weight;						// �ش� index�� weight�� ����
		cout << "\tSuccessed." << endl;
	}
}

// Get weight of two vertices in the graph.
template<typename T>
int Graph<T>::WeightIs(T fromVertex, T toVertex)
{
	int from, to;										// edge�� ��ȯ�� �� ���� index�� �����ϱ� ���� int ���� ����

	from = IndexIs(vertices, fromVertex, numVertices);	// �������� index�� ����
	to = IndexIs(vertices, toVertex, numVertices);		// �������� index�� ����
	
	if (from == -1 && to == -1)							// �������� �ʴ� vertex�� ���� ����ó��	
		return 0;										// return �� 0 ��ȯ
	else
		return edges[from][to];							// �ش� index�� weight�� ��ȯ
}

// Get all adjacent vertices of one vertex in the graph, and insert the queue.
template<typename T>
void Graph<T>::GetToVertices(T vertex, Queue<T>& adgVertices)
{
	int verIndex;										// ������ �Ǵ� vertex�� index�� �����ϱ� ���� int ���� ����
	verIndex = IndexIs(vertices, vertex, numVertices);	// ���� vertex�� index ����
	for (int i = 0; i < numVertices; i++)				// �����ϴ� ��� vertex�� ���Ͽ� for�� ����
	{
		if (edges[verIndex][i] != -1)					// ������ vertex�� weight���� -1(�ʱ�ȭ��)�� ���
			adgVertices.EnQueue(vertices[i]);			// Queue�� �ش� vertex�� �߰�
	}
}

// Clear the mark information.
template<typename T>
void Graph<T>::ClearMarks()
{
	for (int i = 0; i < numVertices; i++)				// �����ϴ� ��� vertex�� ���Ͽ� for�� ����
		marks[i] = false;								// mark�� ���¸� false�� ����
}

// Marking the vertex for escape overlapping to search.
template<typename T>
void Graph<T>::MarkVertex(T vertex)
{
	int verIndex = IndexIs(vertices, vertex, numVertices);	// Marking�� vertex�� index�� ����
	marks[verIndex] = true;									// mark�� ���¸� true�� ����
}

// Check the vertex to marking.
template<typename T>
bool Graph<T>::IsMarked(T vertex)
{
	int verIndex = IndexIs(vertices, vertex, numVertices);	// Marking ���θ� Ȯ���� vertex�� index�� ����

	if (marks[verIndex] == true)							// mark�� ���°� true�� �� true�� ��ȯ
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
	bool found = false;						// ����ó���� ���� boolean ���� ����
	for (int i = 0; i < vertexNum; i++)		
	{
		if (vertices[i] == vertex)			// ��ġ�ϴ� �迭 ���� vertex�� ã���� ���
			return i;						// �� vertex�� �迭 index�� ��ȯ
	}
	if (found == false)						// ��ġ�ϴ� �迭 ���� vertex�� ã�� ������ ���
		return -1;							// return �� -1 ��ȯ
}

// Search the path by breadth-first search.
template <class T>
void BreadthFirstSearch(Graph<T>& graph, T startVertex, T endVertex)
{
	Queue<T> queue;			// ���Ž���� �ʿ��� queue ����
	Queue<T> vertexQueue;	// ����vertexŽ���� ���� vertexQueue ����

	bool found = false;		// Ž������ Ȯ���� ���� boolean ���� ����
	T vertex;				// �Ķ���� �ӽ� vertex ����									
	T item;					// �Ķ���� �ӽ� item ����

	graph.ClearMarks();			// Graph ���� ��� vertex�� mark���� �ʱ�ȭ
	queue.EnQueue(startVertex);	// ���Ž�� queue�� startvertex�� �߰�

	do {
		queue.DeQueue(vertex);		// queue���� vertex���� ����
		if (vertex == endVertex)	// ���� vertex�� ������vertex�� ���� ���
		{
			cout << setw(5) << vertex;	// vertex ��� �� Ž���Ϸ�
			found = true;
		}
		else
		{
			if (!graph.IsMarked(vertex))					// marking�� �ȵ� vertex�� ���
			{		
				graph.MarkVertex(vertex);					// vertex�� marking
				cout << setw(10) << vertex;					// vertex�� ���
				graph.GetToVertices(vertex, vertexQueue);	// ���� vertex�� vertexQueue�� ����
				while (!vertexQueue.IsEmpty())				// ���� vertex�� �������� ���� ������
				{
					vertexQueue.DeQueue(item);				// vertexQueue���� item�� ����
					if (!graph.IsMarked(item))				// marking�� �ȵ� vertex �� ���
						queue.EnQueue(item);				// ���Ž�� queue�� item�� �߰�
				}
			}
		}
	} while (!queue.IsEmpty() && !found);					// ��ΰ� �������� �ʰų� Ž���� �ȵ��� �� ���� ����
	if (!found)		// Ž�� ���п� ���� �޼��� ���
		cout << "\tPath is not found." << endl;
}

// Search the path by depth-first search.
template <class T>
void DepthFirstSearch(Graph<T>& graph, T startVertex, T endVertex)
{
	Stack<T> stack;					// ���Ž���� �ʿ��� stack ����
	Queue<T> vertexQueue;			// ����vertexŽ���� ���� vertexQueue ����

	bool found = false;				// Ž������ Ȯ���� ���� boolean ���� ����
	T vertex;						// �Ķ���� �ӽ� vertex ����	
	T item;							// �Ķ���� �ӽ� item ����

	graph.ClearMarks();				// Graph ���� ��� vertex�� mark���� �ʱ�ȭ
	stack.Push(startVertex);		// ���Ž�� stack�� startvertex�� �߰�

	do
	{
		stack.Pop(vertex);			// stack���� vertex���� ����
		if (vertex == endVertex)	// ���� vertex�� ������vertex�� ���� ���
		{
			cout << setw(5) << vertex;	// vertex ��� �� Ž���Ϸ�
			found = true;
		}
		else
		{
			if (!graph.IsMarked(vertex))					// marking�� �ȵ� vertex�� ���
			{
				graph.MarkVertex(vertex);					// vertex�� marking
				cout << setw(10) << vertex;					// vertex�� ���
				graph.GetToVertices(vertex, vertexQueue);	// ���� vertex�� vertexQueue�� ����
				while (!vertexQueue.IsEmpty())				// ���� vertex�� �������� ���� ������
				{
					vertexQueue.DeQueue(item);				// vertexQueue���� item�� ����
					if (!graph.IsMarked(item))				// marking�� �ȵ� vertex �� ���
						stack.Push(item);					// ���Ž�� stack�� item�� �߰�
				}
			}
		}
	} while (!stack.IsEmpty() && !found);					// ��ΰ� �������� �ʰų� Ž���� �ȵ��� �� ���� ����
	if (!found)			// Ž�� ���п� ���� �޼��� ���
		cout << "\tPath is not found." << endl;
}
