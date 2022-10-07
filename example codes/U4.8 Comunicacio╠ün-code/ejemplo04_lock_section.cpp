#include <stdio.h>
#include "omp.h"

int main()
{
  int N = 100, a[N], b[N];
  omp_set_num_threads(4);
  int nthreads = omp_get_num_threads();
  omp_lock_t locka;
  omp_lock_t lockb;
  void omp_init_lock(omp_lock_t * locka);
  void omp_init_lock(omp_lock_t * lockb);

#pragma omp parallel num_threads(4) shared(a, b, nthreads, locka, lockb)
  {
    int tid = omp_get_thread_num();
#pragma omp sections nowait
    {
#pragma omp section
      {
        printf("s1 tid: %d\n", tid);

        omp_set_lock(&locka);
        printf("locka tid: %d\n", tid);
        for (int i = 0; i < N; i++)
          a[i] = i;

        omp_set_lock(&lockb);
        printf("lockb tid: %d\n", tid);
        for (int i = 0; i < N; i++)
          b[i] = a[i];
        omp_unset_lock(&lockb);
        omp_unset_lock(&locka);
      }

#pragma omp section
      {
        printf("s2 tid: %d\n", tid);
        omp_set_lock(&lockb);
        printf(" lockb tid: %d\n", tid);
        for (int i = 0; i < N; i++)
          b[i] = 2 * i;

        omp_set_lock(&locka);
        printf("locka tid: %d\n", tid);
        for (int i = 0; i < N; i++)
          a[i] = b[i];
        omp_unset_lock(&locka);
        omp_unset_lock(&lockb);
      }
    } /* end of sections */
  }   /* end of parallel region */
  // for (int i=0;i<N;i++) printf("i: %d, a_i: %d, b_i: %d\n",i,a[i],b[i]);
}
