#include <omp.h>
#include <stdio.h>

int main ()  {
int   i, n, chunk;
float a[20], b[20], result, result_p;
n = 20;
result = 0.0;
for (i=0; i < n; i++)
  {
  a[i] = 1.0;
  b[i] = 2.0;
  }
#pragma omp parallel for num_threads(4) reduction(+:result) 
for (i=0; i < n; i++){
    result= result + (a[i] * b[i]);
	printf("result= %f en i= %i del hilo %i\n",result,i,omp_get_thread_num());
	}
printf("result (final)= %f\n",result);
}

