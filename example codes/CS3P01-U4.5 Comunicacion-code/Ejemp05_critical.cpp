#include <omp.h>
#include <stdio.h>
int main()
{
  int x;
  x = 0;
  omp_set_num_threads(8);
#pragma omp parallel
  {
#pragma omp critical
    {
      for (int i = 0; i < 1000; i++)
        x = x + 1;
      printf("valor de x : %i en hilo %i de %i hilos \n", x, omp_get_thread_num(), omp_get_num_threads());
    }
  } /* end of parallel section */
  printf("valor de x : %i  \n", x);
}
