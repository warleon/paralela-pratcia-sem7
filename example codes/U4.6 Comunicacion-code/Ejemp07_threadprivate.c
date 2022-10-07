#include <stdio.h>
#include <omp.h>

int a = 0, b = 0, i, tid;
float x;

int main()
{
  omp_set_num_threads(4);
#pragma omp threadprivate(x)

  /* Explicitly turn off dynamic threads */
  omp_set_dynamic(0);
  printf("1st Parallel Region:\n");
#pragma omp parallel private(b, tid)
  {
    tid = omp_get_thread_num();
    a = tid;
    b = tid;
    x = 1.1 * tid + 1.0;
    printf("Thread %d:   a,b,x= %d %d %f\n", tid, a, b, x);
  } /* end of parallel section */
  printf("Master thread doing serial work here\n");
  printf("2nd Parallel Region:\n");
#pragma omp parallel private(tid)
  {
    tid = omp_get_thread_num();
    printf("Thread %d:   a,b,x= %d %d %f\n", tid, a, b, x);
  }
}
