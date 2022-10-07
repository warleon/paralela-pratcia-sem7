#include <omp.h>
#include <stdio.h>
int main()
{
int x;
x = 0;
omp_set_num_threads(8);
#pragma omp parallel shared(x) 
  {
  #pragma omp critical 
  x = x + 1;
  }  /* end of parallel section */
printf("valor de x : %i  \n",x);

}

