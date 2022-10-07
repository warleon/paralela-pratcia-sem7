#include <omp.h>
#include <cmath>
#include <iostream>

#define N 4
#define THREADS 8
double A[N][N]; // move it here to prevent segfault

void normaliza(double A[N][N])
{
  double suma = 0.0, factor;
  #pragma omp parallel
  {
  #pragma omp for /*nowait*/ reduction(+ \
                                  : suma)
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        suma += A[i][j] * A[i][j];
      }
    }
  // #pragma omp barrier
    factor = 1.0 / sqrt(suma);


  #pragma omp for /*nowait*/
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        A[i][j] = factor * A[i][j];
      }
    }
  }
}

int main()
{
  double t1, t2;
  omp_set_num_threads(THREADS);

  // set initial matrix

  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      A[i][j] = (double)rand();
    }
  }

  // print initial matrix

  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      std::cout << A[i][j] << " ";
    }
    std::cout << std::endl;
  }

  std::cout
      << "----------------------------------------------------------------\n";

  t1 = omp_get_wtime();
  normaliza(A);
  t2 = omp_get_wtime();

  // print normalized matrix

  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      std::cout << A[i][j] << " ";
    }
    std::cout << std::endl;
  }

  printf("tiempo: %f8s\n", (t2 - t1));
  return 0;
}