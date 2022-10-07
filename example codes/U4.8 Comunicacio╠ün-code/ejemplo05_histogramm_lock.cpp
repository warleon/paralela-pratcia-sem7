#include <iostream>
#include <omp.h>
using namespace std;
#define INP_SIZE 1000 //(1<<20)
#define HIST_SIZE 100 // (1<<10)
int hist[HIST_SIZE];
int inp[INP_SIZE];
omp_lock_t lock[HIST_SIZE]; // se define una estructura lock de tamanio HIST_SIZE

int main()
{
  int i, key, sum = 0;
  double t1, t2;
  for (int i = 0; i < INP_SIZE; i++)
    inp[i] = rand() % 100;
  for (int i = 0; i < HIST_SIZE; i++)
    hist[i] = 0;

  printf("arrays creadas\n");
#pragma omp parallel for // private (key)
  for (int i = 0; i < HIST_SIZE; i++)
  {
    omp_init_lock(&(lock[i]));
    //	hist[i]=0;
  }
  t1 = omp_get_wtime();
  int id;
#pragma omp parallel for num_threads(2) // private (id,key)
  for (int i = 0; i < INP_SIZE; i++)
  {
    id = omp_get_thread_num();
    key = inp[i];
    printf("key: %d, inp[i]: %d\n", key, inp[i]);
    omp_set_lock(&(lock[i]));
    hist[key]++;
    if (id == 0)
      printf("hilo: %d, i: %d, key: %d, hist[key]: %d\n", omp_get_thread_num(), i, key, hist[key]);
    omp_unset_lock(&(lock[i]));
  }

  t2 = omp_get_wtime();

  for (int i = 0; i < HIST_SIZE; i++)
  {
    cout << "i: " << i << ",hist_i: " << hist[i] << endl;
    sum += hist[i];
  }

  printf("suma:  %d, tiempo: %f8", sum, (t2 - t1));

  for (int i = 0; i < HIST_SIZE; i++)
    omp_destroy_lock(&(lock[i]));
}
