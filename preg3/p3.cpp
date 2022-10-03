#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

void swap(double *x, double *y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}
int separate(double *x, int low, int high) {
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
void qs(double *z, int zstart, int zend, int firstcall) {
#pragma omp parallel
  {
#pragma omp single
    {
      double pivot;
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
void quicksort(double *A, int n) { return qs(A, 0, n - 1, 1); }

double OpArr(double A[], double B[], double C[], int n) {
  int i, j;
  double s1, s2, a, res;
  suma_de_prefijos(A, n);  // obtiene el array de suma de prefijos de A
  quicksort(B, n);         // ordena el array B
  scan_left(C + x, n);     // acumula los valores de elementos de C mas
  // una constante x
  for (i = 0; i < n; i++) { /* primer bucle for */
    s1 = 0;
    for (j = 0; j < n; j++) s1 += A[j] * B[j];
    for (j = 0; j < n; j++) A[j] *= s1;
  }
  for (i = 0; i < n; i++) { /* segundo bucle for */
    s2 = 0;
    for (j = 0; j < n; j++) s2 += B[j] * C[j];
    for (j = 0; j < n; j++) C[j] *= s2;
  }
  /* calculo final */
  a = s1 / s2;
  res = 0;
  for (i = 0; i < n; i++) res += a * C[i];
  return res;
}