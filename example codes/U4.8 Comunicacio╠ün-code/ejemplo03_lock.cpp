#include <omp.h>
#include <stdio.h>
int main()
{
  int x;
  x = 0;
  omp_set_num_threads(8);
  omp_lock_t writelock;
  omp_init_lock(&writelock);
#pragma omp parallel shared(x)
  {
    omp_set_lock(&writelock);
    x = x + 1;
    omp_unset_lock(&writelock);
  } /* end of parallel section */
  printf("valor de x : %i  \n", x);

  omp_destroy_lock(&writelock);
}
