#include "stdio.h"
#include "omp.h"
static long num_steps = 100000;
double step;
int THREADS = 8;
int main()
{
	int n =0;
	double pi = 0.0;
	double sum[THREADS];
	step = 1.0 / (double)num_steps;
	omp_set_num_threads(THREADS);
	#pragma omp parallel
	{ 
		int i;

		n = omp_get_num_threads();
		int cur_n = omp_get_thread_num();

		printf("sum start: %f\n", sum[cur_n] = 0.0);

		// int start = (1*((bool)cur_n==true)) +(int)(num_steps / (double) n) * cur_n;
		// int stop = (int)(num_steps / (double) n) * (cur_n+1);
		
		// printf("thread: %d,start/stop: %d, %d\n", cur_n, start, stop);
		// for (i = start; i <= stop; i++)
		for (i = cur_n; i <= num_steps; i=i+n) //round robin distribution, cool!
		{
			double x = (i + 0.5) * step;
			sum[cur_n] = sum[cur_n] + 4.0 / (1.0 + x * x);
		}
		printf("sum end: %f\n", sum[cur_n]);
	}
	double sums = 0.0;
	for (int i=0; i <n; i++) sums += sum[i];
	pi = step * sums;
	printf("%f\n", pi);
}