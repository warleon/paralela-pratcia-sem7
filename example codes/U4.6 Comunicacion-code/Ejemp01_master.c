/*
 * @@name:	master.1c
 * @@type:	C
 * @@compilable:	yes
 * @@linkable:	no
 * @@expect:	success
 */
#include <stdio.h>
#include <omp.h>

float average(float xo, float x, float xo1) { return (xo + x + xo1) / 3; };

void master_example(float *x, float *xold, int n, float tol)
{
  int c, i, toobig;
  float error, y;
  c = 0;
#pragma omp parallel
  {
    do
    {
#pragma omp for
      for (i = 1; i < n - 1; ++i)
      {
        xold[i] = x[i];
      }
#pragma omp single
      {
        toobig = 0;
      }
#pragma omp for reduction(+ \
                          : toobig)
      for (i = 1; i < n - 1; ++i)
      {
        y = x[i];
        x[i] = average(xold[i - 1], x[i], xold[i + 1]);
        error = y - x[i];
        printf("i: %d , hilo: %d\n ", i, omp_get_thread_num());
        if (error > tol || error < -tol)
          ++toobig;
      }
#pragma omp master
      {
        ++c;
        printf("iteration %d, toobig=%d\n", c, toobig);
      }
    } while (toobig > 0);
  }
}

int main()
{
  int i, n = 12;
  float tol = 0.01, x[n], xold[n];

  omp_set_num_threads(8);

  for (i = 0; i < n; i++)
  {
    x[i] = i;
    xold[i] = 0;
  }
  master_example(x, xold, n, tol);
}
