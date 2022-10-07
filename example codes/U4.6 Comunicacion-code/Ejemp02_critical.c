/*
 * @@name:	critical.1c
 * @@type:	C
 * @@compilable:	yes
 * @@linkable:	no
 * @@expect:	success
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

int iRand(int iMin, int iMax)
{
  // srand(time(NULL));
  return rand() % iMax + iMin;
}

int dequeue(float *a, int n)
{
  printf("deque por hilo %i \n", omp_get_thread_num());
  return a[iRand(0, n - 1)];
};
// int dequeue(float *a, int n){return a[1];};
void work(int i, float *a)
{
  a[i] = a[i] * 2;
  printf("thread: %d i: %d, a: %f\n", omp_get_thread_num(), i, a[i]);
};

void critical_example(float *x, float *y, int n)
{
  int ix_next, iy_next;

#pragma omp parallel num_threads(2) shared(x, y) private(ix_next, iy_next)
  {
#pragma omp critical(xaxis)
    ix_next = dequeue(x, n);
    work(ix_next, x);

#pragma omp critical(yaxis)
    iy_next = dequeue(y, n);
    work(iy_next, y);
  }
}

int main()
{
  int i, n = 10;
  float x[n], y[n];

  for (i = 0; i < n; i++)
  {
    x[i] = i;
    y[i] = i;
    printf("x: %f, y: %f \n", x[i], y[i]);
  }

  critical_example(x, y, n);

  for (i = 0; i < n; i++)
  {
    printf("x: %f, y: %f \n", x[i], y[i]);
  }
}
