#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

void swap(int *x, int *y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}
int separate(int *x, int low, int high) {
  int i, pivot, last;
  pivot = x[low];
  swap(x + low, x + high);
  last = low;
  for (i = low; i < high; i++) {
    if (x[i] <= pivot) {
      swap(x + last, x + i);
      last += 1;
    }
  }
  swap(x + last, x + high);
  return last;
}
void qs(int *z, int zstart, int zend, int firstcall) {
#pragma omp parallel
  {
#pragma omp single
    {
      int pivot;
      if (firstcall == 1) {
#pragma omp task
        qs(z, 0, zend, 0);
      } else {
        if (zstart < zend) {
          pivot = separate(z, zstart, zend);
#pragma omp task
          qs(z, zstart, pivot - 1, 0);
#pragma omp task
          qs(z, pivot + 1, zend, 0);
        }
      }
    }
  }
}
int main(int argc, char **argv) {
  int i, n, *w;
  n = atoi(argv[1]);
  w = (int *)malloc(n * sizeof(int));
  for (i = 0; i < n; i++) w[i] = rand() % 100;
  if (n < 25)
    for (i = 0; i < n; i++) printf("%d,", w[i]);

  qs(w, 0, n - 1, 1);

  std::cout << "\n================================\n";
  if (n < 25)
    for (i = 0; i < n; i++) printf("%d,", w[i]);

  std::cout << std::endl;
  return 0;
}