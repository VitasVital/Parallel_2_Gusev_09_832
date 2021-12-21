#include <array>
#include <iostream>
#include<fstream>
#include <sstream>
#include<string>
#include <vector>

#include <queue> // �������
#include <stack> // ����

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

int OptimalRoute(int req)
{
	int count_nodes = 0;
	ifstream f("neighboring_nodes.txt");
	int mas[N][N];
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			f >> mas[i][j];
	
	queue<int> Queue;
	stack<Edge> Edges;
	Edge e;
	int nodes[7]; // ������� �����
	for (int i = 0; i < 7; i++) // ������� ��� ������� ����� 0
		nodes[i] = 0;
	Queue.push(0); // �������� � ������� ������ �������
	while (!Queue.empty())
	{
		int node = Queue.front(); // ��������� �������
		Queue.pop();
		nodes[node] = 2; // �������� �� ��� ����������
		for (int j = 0; j < 7; j++)
		{
			if (mas[node][j] == 1 && nodes[j] == 0)
			{ // ���� ������� ������� � �� ����������
				Queue.push(j); // ��������� �� � �������
				nodes[j] = 1; // �������� ������� ��� ������������
				e.begin = node; e.end = j;
				Edges.push(e);
				if (node == req) break;
			}
		}
	}
	/*cout << "���� �� ������� " << req << endl;
	cout << req;*/
	while (!Edges.empty()) {
		e = Edges.top();
		Edges.pop();
		if (e.end == req) {
			req = e.begin;
			//cout << " <- " << req;
			count_nodes += 1;
		}
	}
	//cout << endl;
	return count_nodes;
}


int main(int argc, char** argv)
{
	setlocale(LC_CTYPE, "Russian");
	
	int size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	////��������� ������� ���������
	//ifstream f("neighboring_nodes.txt");
	//int a[N][N];
	//for (int i = 0; i < N; i++)
	//	for (int j = 0; j < N; j++)
	//		f >> a[i][j];

	////������� ������� ���������
	//show_read_arr(a);
	
	if (rank == 0)
	{
		//OptimalRoute();
		int count_routes[N] = { 0 };
		for (int i = 1; i < N; i++)
		{
			count_routes[i] = OptimalRoute(i);
		}

		cout << "���������� ����� �� �����:" << endl;
		for (int i = 1; i < N; i++)
		{
			cout << i << "=" << count_routes[i] << endl;
		}
		
		int* arr = new int[size];
		for (int i = 0; i < size; i++) arr[i] = i;
		for (int i = 1; i < size; i++) MPI_Send(arr, i, MPI_INT, i, 5, MPI_COMM_WORLD);
		/*buf - ������ �� ����� �� �������� ����� ������, ������� �� ����������.� ������ �������� ������ �� ������ �������.
		count - ���������� ��������� � ���� �������, ���� ���������� ������ ����������, �� ����� 1.
		datatype - ��� ��� ����� ���������, � MPI ���� ���� ���������������� ���� ������ ������� ���������� � �++.�� ������� � ������� ���� ������.
		dest - ����� �������� ���� ���������� ���������.
		msgtag - ID ���������(����� ����� �����)
		comm - ������������ � ������� ��������� ������� �������� �� ���������� ���������.*/
	}
	else
	{
		int count;
		MPI_Status status;

		MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_INT, &count);
		int* buf = new int[count];

		MPI_Recv(buf, count, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		/*buf - ������ �� ����� �� �������� ����� ��������� ������������ ������.
		count - ������������ ���������� ����������� ���������.
		datatype - ��� ������ ���������������� � MPI(�� �������� � Send).
		source - ����� �������� ������� �������� ���������.
		tag - ID ��������� ������� �� ���������(����� ����� �����)
		comm - ������������ � ������� ��������� ������� �� �������� �������� ���������.
		status - ���������, ������������ � MPI ������� ������ ���������� � ��������� � ������ �� ����������.*/

		cout << "Process:" << rank << " || Count: " << count << " || Array: ";
		//show_arr(buf, count);
	}
	MPI_Finalize();
	return 0;
}