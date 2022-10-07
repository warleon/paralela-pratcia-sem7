#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// #define THREADS 3
// #define N 106

int N, THREADS;

void swap(int *x, int *y)
{
  int temp = *x;
  *x = *y;
  *y = temp;
}

int separate(int *x, int low, int high)
{
  int i, pivot, last;
  pivot = x[low];
  swap(x + low, x + high);
  last = low;
  for (i = low; i < high; i++)
  {
    if (x[i] <= pivot)
    {
      swap(x + last, x + i);
      last += 1;
    }
  }
  swap(x + last, x + high);
  return last;
}

void qs(int *z, int zstart, int zend)
{
  // printf("hilo en qs: %i, nivel: %i, size: %i\n",
  //        omp_get_thread_num(), omp_get_active_level(), (zend - zstart));
  int pivot;
  {
    {
      if (zstart < zend)
      {
        pivot = separate(z, zstart, zend);

#pragma omp task shared(z) if ((zend - zstart) > N / THREADS)
        qs(z, zstart, pivot - 1);

#pragma omp task shared(z) if ((zend - zstart) > N / THREADS)
        qs(z, pivot + 1, zend);

#pragma omp taskwait
      }
    }
  }
}

void pqs(int *z, int zend)
{
#pragma omp parallel
  {
#pragma omp single
    qs(z, 0, zend - 1);
  }
}

int main(int argc, char **argv)
{
  std::cin >> N >> THREADS;

  double t1, t2;
  omp_set_num_threads(THREADS);
  omp_set_nested(1);
  omp_set_dynamic(0);

  int i, *w;

  // if (argc < 2)
  // n = N;
  // else
  //   n = atoi(argv[1]);

  w = (int *)malloc(N * sizeof(int));

  for (i = 0; i < N; i++)
    w[i] = rand() % 100;

  // if (N < 25){
  //   for (i = 0; i < N; i++)
  //     printf("%d,", w[i]);
  //   printf("\n\n");
  // }

  t1 = omp_get_wtime();
  pqs(w, N);
  t2 = omp_get_wtime();

  // std::cout << "\n================================\n";
  // if (N < 25)
  //   for (i = 0; i < N; i++)
  //     printf("%d,", w[i]);

  // printf("\ntiempo: %f8s\n", (t2 - t1));
  
  printf("%f8", (t2 - t1));
  return 0;
}