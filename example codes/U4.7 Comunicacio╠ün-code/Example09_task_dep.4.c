#include <omp.h>
#include <stdio.h>
#include <unistd.h>

int main()
{
  int a, b, c;

#pragma omp parallel
  {
#pragma omp master
    {
#pragma omp task depend(out \
                        : a)
      {
        printf("Task 1\n");
      }

#pragma omp task depend(out \
                        : b)
      {
        printf("Task 2\n");

#pragma omp task depend(out \
                        : a, b, c)
        {
          printf("Task 5\n");
        }
      }

#pragma omp task depend(in                 \
                        : a, b) depend(out \
                                       : c)
      {
        printf("Task 3\n");
      }
#pragma omp task depend(in \
                        : c)
      {
        printf("Task 4\n");
      }
    }
  }
  return 0;
}
