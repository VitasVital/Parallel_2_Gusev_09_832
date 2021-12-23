#include <array>
#include <iostream>
#include<fstream>
#include <sstream>
#include<string>
#include <vector>

#include <queue> // очередь
#include <stack> // стек

#include "mpi.h"

using namespace std;
struct Edge {
	int begin;
	int end;
};

const int N = 6;

void show_read_arr(int arr[][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}
}

void OptimalRoute(int req, int root, int &count_nodes, int mas[N][N], int routes[N])
{
	//ifstream f("neighboring_nodes_2.txt");
	//int mas[N][N];
	//for (int i = 0; i < N; i++)
	//	for (int j = 0; j < N; j++)
	//		f >> mas[i][j];
	
	queue<int> Queue;
	stack<Edge> Edges;
	Edge e;
	int nodes[7]; // вершины графа
	for (int i = 0; i < 7; i++) // исходно все вершины равны 0
		nodes[i] = 0;
	Queue.push(0); // помещаем в очередь первую вершину
	while (!Queue.empty())
	{
		int node = Queue.front(); // извлекаем вершину
		Queue.pop();
		nodes[node] = 2; // отмечаем ее как посещенную
		for (int j = 0; j < 7; j++)
		{
			if (mas[node][j] == 1 && nodes[j] == 0)
			{ // если вершина смежная и не обнаружена
				Queue.push(j); // добавляем ее в очередь
				nodes[j] = 1; // отмечаем вершину как обнаруженную
				e.begin = node; e.end = j;
				Edges.push(e);
				if (node == req) break;
			}
		}
	}
	cout << "The path to the top " << req << endl; //путь до вершины
	cout << req;
	routes[0] = req;
	int index = 1;
	while (!Edges.empty()) {
		e = Edges.top();
		Edges.pop();
		if (e.end == req) {
			req = e.begin;
			cout << " <- " << req;
			routes[index] = req; //записываем номер узла в машрут
			index += 1;
			count_nodes += 1;
		}
	}
	cout << endl;
}

//void MyMPIBcast(int root, int* number_to_send)
//{
//	MPI_Status status;
//	int k;
//
//	MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//	MPI_Get_count(&status, MPI_INT, &k);
//
//	MPI_Recv(buf, k, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//
//
//
//	MPI_Send(arr, number_to_send, MPI_INT, i, 5, MPI_COMM_WORLD);
//}


int main(int argc, char** argv)
{
	setlocale(LC_CTYPE, "Russian");

	
	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size); //количество процессов
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); //номер процесса


	int k = 6;
	int count_routes[N] = { 0 };
	int root = 4;
	cout << "Root node = " << root << endl;
	/*int mas[N][N] = { {0, 1, 1, 0 },
						{1, 0, 0, 0 },
						{1, 0, 0, 1 },
						{0, 0, 1, 0 } };*/

	int mas[N][N] = { {0, 1, 1, 0, 0, 0},
						{1, 0, 0, 0, 0, 0},
						{1, 0, 0, 1, 1, 0},
						{0, 0, 1, 0, 0, 0},
						{0, 0, 1, 0, 0, 1},
						{0, 0, 0, 0, 1, 0} };

	
	int swap = mas[0][root];
	mas[0][root] = mas[root][root];
	mas[root][root] = swap;
	for (int j = 0; j < N; j++) //делаем root корневым, поэтому root становится равен нулю
	{
		swap = mas[root][j];
		mas[root][j] = mas[0][j];
		mas[0][j] = swap;
	}
	
	if (rank == 0)
	{
		int routes[N][N] = { {-1, -1, -1, -1, -1, -1},
						{-1, -1, -1, -1, -1, -1},
						{-1, -1, -1, -1, -1, -1},
						{-1, -1, -1, -1, -1, -1},
						{-1, -1, -1, -1, -1, -1},
						{-1, -1, -1, -1, -1, -1} };

		/*int routes[N][N] = { {-1, -1, -1, -1 },
							{-1, -1, -1, -1 },
							{-1, -1, -1, -1 },
							{-1, -1, -1, -1 } };*/

		for (int i = 1; i < N; i++)
		{
			OptimalRoute(i, root, count_routes[i], mas, routes[i]);
		}

		cout << "Number of edges to nodes:" << endl; //Количество ребер в узлах
		for (int i = 1; i < N; i++)
		{
			cout << "Node " << i << " = " << count_routes[i] << endl;
		}

		cout << "Routes:" << endl;

		show_read_arr(routes);

		//MyMPIBcast(root, &k);

		/*int* arr = new int[size];
		for (int i = 0; i < size; i++) arr[i] = i;
		for (int i = 1; i < size; i++) MPI_Send(arr, i, MPI_INT, i, 5, MPI_COMM_WORLD);*/
		/*buf - ссылка на адрес по которому лежат данные, которые мы пересылаем.В случае массивов ссылка на первый элемент.
		count - количество элементов в этом массиве, если отправляем просто переменную, то пишем 1.
		datatype - тут уже чутка посложнее, у MPI есть свои переопределенные типы данных которые существуют в С++.Их таблицу я приведу чуть дальше.
		dest - номер процесса кому отправляем сообщения.
		msgtag - ID сообщения(любое целое число)
		comm - Коммуникатор в котором находится процесс которому мы отправляем сообщение.*/

		int neighboring_nodes[N][2]; //находим соседние узлы узла
		int index = 0;
		for (int i = 1; i < N; i++)
		{
			if (routes[i][1] == root)
			{
				neighboring_nodes[index][0] = routes[i][0];
				index += 1;
			}
		}
		for (int ind = 0; ind < index; ind++)
		{
			for (int i = 1; i < N; i++)
			{
				for (int j = 0; j < N; j++)
				{
					if (neighboring_nodes[index][0] == routes[i][j])
					{
						neighboring_nodes[index][1] += 1; //считаем количество маршрутов у соседних узлов
					}
				}
			}
		}
		int swap;
		for (int i = 0; i < index; i++) //сортируем узлы по убыванию
		{
			for (int j = i; j < index; j++)
			{
				if (neighboring_nodes[i][1] < neighboring_nodes[j][1])
				{
					swap = neighboring_nodes[i][1];
					neighboring_nodes[i][1] = neighboring_nodes[j][1];
					neighboring_nodes[j][1] = swap;

					swap = neighboring_nodes[i][0];
					neighboring_nodes[i][0] = neighboring_nodes[j][0];
					neighboring_nodes[j][0] = swap;
				}
			}
		}
		for (int i = 1; i < index; i++)
		{
			MPI_Send(routes, 36, MPI_INT, neighboring_nodes[i][0], 5, MPI_COMM_WORLD);
		}
	}
	else
	{
		int count;
		
		MPI_Status status;

		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &count);
		int routes[N][N];
		
		MPI_Recv(routes, count, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		int source = status.MPI_SOURCE;
		/*buf - ссылка на адрес по которому будут сохранены передаваемые данные.
		count - максимальное количество принимаемых элементов.
		datatype - тип данных переопределенный в MPI(по аналогии с Send).
		source - номер процесса который отправил сообщение.
		tag - ID сообщения которое мы принимаем(любое целое число)
		comm - Коммуникатор в котором находится процесс от которого получаем сообщение.
		status - структура, определенная в MPI которая хранит информацию о пересылке и статус ее завершения.*/

		//cout << "Process:" << rank << " || Size: " << size << " || Count: " << count << " || Array: ";
		
		//show_arr(buf, count);

		int neighboring_nodes[N][2]; //находим соседние узлы узла
		int index = 0;
		for (int i = 1; i < N; i++)
		{
			if (routes[i][1] == source)
			{
				neighboring_nodes[index][0] = routes[i][0];
				index += 1;
			}
		}

		if (index > 0)
		{
			for (int ind = 0; ind < index; ind++)
			{
				for (int i = 0; i < N; i++)
				{
					for (int j = 0; j < N; j++)
					{
						if (neighboring_nodes[index][0] == routes[i][j])
						{
							neighboring_nodes[index][1] += 1; //считаем количество маршрутов у соседних узлов
						}
					}
				}
			}
			int swap;
			for (int i = 0; i < index; i++) //сортируем узлы по убыванию
			{
				for (int j = i; j < index; j++)
				{
					if (neighboring_nodes[i][1] < neighboring_nodes[j][1])
					{
						swap = neighboring_nodes[i][1];
						neighboring_nodes[i][1] = neighboring_nodes[j][1];
						neighboring_nodes[j][1] = swap;

						swap = neighboring_nodes[i][0];
						neighboring_nodes[i][0] = neighboring_nodes[j][0];
						neighboring_nodes[j][0] = swap;
					}
				}
			}
			for (int i = 0; i < index; i++)
			{
				MPI_Send(routes, 36, MPI_INT, neighboring_nodes[i][0], 5, MPI_COMM_WORLD);
			}
		}
	}
	MPI_Finalize();
}