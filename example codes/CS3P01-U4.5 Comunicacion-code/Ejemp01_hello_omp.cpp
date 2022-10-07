#include <iostream>
#include <omp.h>
using namespace std;

int main()
{
	int nthreads = 0;

#pragma omp parallel num_threads(8)
	{
		printf("Peekaboo desde el hilo = %d \n", omp_get_thread_num());
		nthreads = omp_get_num_threads();
	}
	cout << "Listo!, trabajado con " << nthreads << " hilos desde el maestro" << endl;
}
