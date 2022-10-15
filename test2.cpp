// #include "stdio.h"
// #include "omp.h"
// static long num_steps = 100000;
// double step;
// int THREADS = 8;

// int main()
// {
// 	int n =0;
// 	double pi = 0.0;
// 	double sum[THREADS];
// 	step = 1.0 / (double)num_steps;
// 	omp_set_num_threads(THREADS);

//   int i, tid;
//   double suma=0.0;
// 	#pragma omp parallel private(i,tid) reduction(+:suma) 
// 	{ 
// 		tid = omp_get_num_threads();
//     suma = 0.0;
//     for(i=0;i<10;i++){
//       suma += tid*i;
//     }
// 	}
//   printf("sum: %f\n", suma);
// }

#include "stdio.h"
#include "omp.h"
#include "cmath"
static long num_steps = 100000;
double step;
int THREADS = 8;

int main(){
  printf("a\n");
	int n = 0;
	double pi = 0.0;
	double sum[THREADS];
	step = 1.0 / (double)num_steps;
	omp_set_num_threads(THREADS);

  
  int N = 100;
  int A[N], B[N];
  for (int n = 0; n < N; n++)
  {
    A[n] = rand();
    B[n] = rand();
  }

	#pragma omp parallel for firstprivate(A) shared(B)
    for(int i=0; i<N;i++){
      int tid = omp_get_thread_num();
      
      B[i] = pow(sin(A[i]), 2);
       printf("Thread: %d, cur_it: %d\n", tid, i);
    }    
}