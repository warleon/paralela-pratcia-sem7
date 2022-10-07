#include <iostream>
#include <omp.h>
using namespace std;
#define INP_SIZE 10000 //(1<<26)
#define HIST_SIZE 100  //(1<<20)
int hist[HIST_SIZE];
int inp[INP_SIZE];

int main()
{
  int i, key, sum = 0;
  double t1, t2;

  for (int i = 0; i < INP_SIZE; i++)
  {
    inp[i] = rand() % 100;
  }

  t1 = omp_get_wtime();
#pragma omp parallel for private(key) reduction(+ \
                                                : sum)
  for (int i = 0; i < INP_SIZE; i++)
  {
    key = inp[i];
#pragma omp atomic
    hist[key]++;
  }
  t2 = omp_get_wtime();

  for (int i = 0; i < HIST_SIZE; i++)
    sum += hist[i];

  printf("suma:  %d, tiempo: %f8", sum, (t2 - t1));
}
