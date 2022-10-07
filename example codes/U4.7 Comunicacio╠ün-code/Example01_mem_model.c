/*
 * @@name:	mem_model.1c
 * @@type:	C
 * @@compilable:	yes
 * @@linkable:	yes
 * @@expect:	rt-error
 */
#include <stdio.h>
#include <omp.h>
int main()
{
  int x;
  x = 2;
#pragma omp parallel num_threads(8) shared(x)
  {
    if (omp_get_thread_num() == 0)
    {
      x = 5;
    }
    else
    {
      /* Print 1: lectura concurrente de x */
      printf("1: hilo %d: x = %d\n", omp_get_thread_num(), x);
    }

#pragma omp barrier

    if (omp_get_thread_num() == 0)
    {
      /* Print 2 */
      printf("2: hilo %d: x = %d\n", omp_get_thread_num(), x);
    }
    else
    {
      /* Print 3 */
      printf("3: hilo %d: x = %d\n", omp_get_thread_num(), x);
    }
  }
  return 0;
}
