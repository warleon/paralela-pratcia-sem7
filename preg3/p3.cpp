#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include <cmath>
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
void suma_de_prefijos(double *A, int n) {
  // sweep up
  long dmax = log2(n - 1);
  for (long d = 0; d < dmax; d++) {
#pragma omp parallel for
    for (long long k = 0; k < n; k += (1 << (d + 1))) {
      A[k + (1 << (d + 1)) - 1] = A[k + (1 << d) - 1] + A[k + (1 << d)];
    }
  }
  // sweep down
  A[n - 1] = 0;
  for (long d = dmax; d >= 0; d--) {
#pragma omp parallel for
    for (long long k = 0; k < n; k += ((1 << d) + 1)) {
      auto temp = A[k + (1 << d) - 1];
      A[k + (1 << d) - 1] = A[k + (1 << d)];
      A[k + (1 << d)] += temp;
    }
  }
}
void scan_left(double *C, int n, double &x) {
  long dmax = log2(n - 1);
  auto A = new double[n];
#pragma omp parallel for
  for (int j = 0; j < n; j++) {
    A[j] = C[j];
  }
  for (long d = 0; d < dmax; d++) {
#pragma omp parallel for
    for (long long k = 0; k < n; k += (1 << (d + 1))) {
      A[k + (1 << (d + 1)) - 1] = A[k + (1 << d) - 1] + A[k + (1 << d)];
    }
  }
  x += A[n - 1];
  delete[] A;
}

double multMod(double *A, double *B, int n) {
  double s1;
  for (int i = 0; i < n; i++) { /* primer bucle for */
    s1 = 0;
#pragma omp parallel for reduction(+ : s1)
    for (int j = 0; j < n; j++) s1 += A[j] * B[j];
#pragma omp parallel for
    for (int j = 0; j < n; j++) A[j] *= s1;
  }
  return s1;
}

double OpArr(double A[], double B[], double C[], int n) {
  int i, j;
  double s1, s2, a, res;
  double x = 1.0;
  suma_de_prefijos(A, n);  // obtiene el array de suma de prefijos de A
  quicksort(B, n);         // ordena el array B
  scan_left(C, n, x);      // acumula los valores de elementos de C mas
  // una constante x
  s1 = multMod(A, B, n);
  s2 = multMod(C, B, n);
  /* calculo final */
  a = s1 / s2;
  res = 0;
#pragma omp parallel for reduction(+ : res)
  for (i = 0; i < n; i++) res += a * C[i];
  return res;
}
int main() {
  const int n = 1 << 14;
  double a[n], b[n], c[n];
  OpArr(a, b, c, n);

  return 0;
}