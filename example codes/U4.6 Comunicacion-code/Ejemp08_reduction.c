#include <stdio.h>
#include <omp.h>

int main(void)
{
  int a, i;

#pragma omp parallel private(i)
  {
    a = 0;
#pragma omp for reduction(+ \
                          : a)
    for (i = 0; i < 50; i++)
    {
      a += i;
      printf("valor de %d en el hilo %d\n", a, omp_get_thread_num());
    }
#pragma omp single
    printf("La suma es %d\n", a);
  }
  return 0;
}
