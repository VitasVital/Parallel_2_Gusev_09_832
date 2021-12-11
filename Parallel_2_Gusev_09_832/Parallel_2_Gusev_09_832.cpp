#include <iostream>
#include "mpi.h"

using namespace std;

int main(int argc, char* argv[])
{
	/*int errCode;

	if ((errCode = MPI_Init(&argc, &argv)) != 0)
	{
		return errCode;
	}

	cout << "hello world\n";


	MPI_Finalize();
	return 0;*/

	printf("Before MPI_INIT\n");
	MPI_Init(&argc, &argv);
	printf("Parallel sect\n");
	MPI_Finalize();
	printf("After MPI_FINALIZE\n");
	return 0;
}