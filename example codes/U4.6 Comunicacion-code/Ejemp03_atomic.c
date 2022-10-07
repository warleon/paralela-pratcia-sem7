/*
 * @@name:	atomic.1c
 * @@type:	C
 * @@compilable:	yes
 * @@linkable:	yes
 * @@expect:	success
 */
#include <stdio.h>
#include <omp.h>

float work1(int i)
{
  return 1.0 * i;
}

float work2(int i)
{
  return 2.0 * i;
}

void atomic_example(float *x, float *y, int *index, int n)
{
  int i;

#pragma omp parallel for shared(x, y, index, n)
  for (i = 0; i < n; i++)
  {
#pragma omp atomic update
    x[index[i]] += work1(i);
  if (omp_get_thread_num() == 0)
    printf("%d "
            " %d "
            " %f hilo: %d de %d\n",
            i, index[i], x[index[i]], omp_get_thread_num(), omp_get_num_threads());
    y[i] += work2(i);
  }
}

int main()
{
  int n = 10;
  float x[n];
  float y[10 * n];
  int index[10 * n];
  int i;
  float value = 333;

  omp_set_num_threads(8);
  for (i = 0; i < 10 * n; i++)
  {
    index[i] = i % n;
    y[i] = 0.0;
  }
  for (i = 0; i < n; i++)
    x[i] = 0.0;

  printf("atomic update:\n");
  atomic_example(x, y, index, 10 * n);

#pragma omp single
  for (i = 0; i < n; i++)
  {
    printf("i: %d, x: %f, y: %f value: %f\n", i, x[i], y[i], value);
  }
  return 0;
}
