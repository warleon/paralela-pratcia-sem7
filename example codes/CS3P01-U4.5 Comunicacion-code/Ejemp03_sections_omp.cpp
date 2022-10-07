#include <omp.h>
#include <iostream>
using namespace std;
#define N 10

int main()
{
	int i, j;
	float a[N], b[N], c[N], d[N];

	for (i = 0; i < N; i++)
	{
		a[i] = i * 1;
		b[i] = i + 2;
	}
	omp_set_num_threads(4);

#pragma omp parallel shared(a,b,c,d) private(i)
	{
#pragma omp sections // nowait
		{
#pragma omp section
			{
				for (i = 0; i < N; i++)
				{
					c[i] = a[i] + b[i];
					printf("seccion 1. Hilo: %i it: %i \n ", omp_get_thread_num(), i);
				}
			}
#pragma omp section
			{
				for (i = 0; i < N; i++)
				{
					d[i] = a[i] * b[i];
					printf("seccion 2. Hilo: %i it: %i \n ", omp_get_thread_num(), i);
				}
			}
		} /* end of sections */
	}		/* end of parallel section */

	for (i = 0; i < N; i++)
		cout << a[i] << "\t" << b[i] << "\t" << c[i] << "\t" << d[i] << endl;
}
