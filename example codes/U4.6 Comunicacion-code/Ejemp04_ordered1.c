/*
 * @@name:	ordered.1c
 * @@type:	C
 * @@compilable:	yes
 * @@linkable:	yes
 * @@expect:	success
 */
#include <stdio.h>
#include <omp.h>

void work1(int k)
{
#pragma omp ordered
  printf(" %d %d\n", k, omp_get_thread_num());
}

void work2(int k)
{
#pragma omp ordered
  printf(" %d %d\n", k, omp_get_thread_num());
}

void ordered_example(int lb, int ub, int stride)
{
  int i;

#pragma omp parallel for ordered schedule(static)
  for (i = lb; i < ub; i += stride)
    work1(i);
#pragma omp parallel for ordered schedule(dynamic)
  for (i = lb; i < ub; i += stride)
    work2(i);
}

int main()
{
  ordered_example(0, 10, 1);
  return 0;
}
