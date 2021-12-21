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

void OptimalRoute(int req, int root, int &count_nodes)
{
	ifstream f("neighboring_nodes_2.txt");
	int mas[N][N];
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			f >> mas[i][j];

	int k = mas[0][root];
	mas[0][root] = mas[root][root];
	mas[root][root] = k;
	for (int j = 0; j < N; j++) //делаем root корневым
	{
		k = mas[root][j];
		mas[root][j] = mas[0][j];
		mas[0][j] = k;
	}
	
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
	while (!Edges.empty()) {
		e = Edges.top();
		Edges.pop();
		if (e.end == req) {
			req = e.begin;
			cout << " <- " << req;
			count_nodes += 1;
		}
	}
	cout << endl;
}

void MyMPIBcast(int root, int* number_to_send)
{
	
}


int main(int argc, char** argv)
{
	setlocale(LC_CTYPE, "Russian");
	
	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);


	int k = 1;
	MyMPIBcast(5, &k);
	if (rank == 0)
	{
		int count_routes[N] = { 0 };
		int root = 4;
		for (int i = 1; i < N; i++)
		{
			OptimalRoute(i, root, count_routes[i]);
		}

		cout << "Number of edges to nodes:" << endl; //Количество ребер в узлах
		for (int i = 1; i < N; i++)
		{
			cout << i << "=" << count_routes[i] << endl;
		}

		int* arr = new int[size];
		for (int i = 0; i < size; i++) arr[i] = i;
		for (int i = 1; i < size; i++) MPI_Send(arr, i, MPI_INT, i, 5, MPI_COMM_WORLD);
		/*buf - ссылка на адрес по которому лежат данные, которые мы пересылаем.В случае массивов ссылка на первый элемент.
		count - количество элементов в этом массиве, если отправляем просто переменную, то пишем 1.
		datatype - тут уже чутка посложнее, у MPI есть свои переопределенные типы данных которые существуют в С++.Их таблицу я приведу чуть дальше.
		dest - номер процесса кому отправляем сообщения.
		msgtag - ID сообщения(любое целое число)
		comm - Коммуникатор в котором находится процесс которому мы отправляем сообщение.*/
	}
	else
	{
		int count;
		MPI_Status status;

		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &count);
		int* buf = new int[count];

		MPI_Recv(buf, count, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		/*buf - ссылка на адрес по которому будут сохранены передаваемые данные.
		count - максимальное количество принимаемых элементов.
		datatype - тип данных переопределенный в MPI(по аналогии с Send).
		source - номер процесса который отправил сообщение.
		tag - ID сообщения которое мы принимаем(любое целое число)
		comm - Коммуникатор в котором находится процесс от которого получаем сообщение.
		status - структура, определенная в MPI которая хранит информацию о пересылке и статус ее завершения.*/

		//cout << "Process:" << rank << " || Count: " << count << " || Array: ";
		//show_arr(buf, count);
	}
	MPI_Finalize();
}