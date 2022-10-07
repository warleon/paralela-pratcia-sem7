/*
* @@name:	private.1c
* @@type:	C
* @@compilable:	yes
* @@linkable:	yes
* @@expect:	success
*/
#include <stdio.h>
#include <omp.h>
#include <assert.h>

int main()
{
  int i, j;

  i = 1;
  j = 2;

omp_set_num_threads(4);
  #pragma omp parallel   
  {
    i = 3;
    j = j + 2;
#pragma omp single
  printf("parallel i: %i j: %i \n",i,j);
  }

  printf("i: %i j: %i \n",i,j);

  return 0;
}
