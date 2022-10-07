// void normaliza(double A[N][N])
// {
//   int i, j;
//   double suma = 0.0, factor;
//   for (i = 0; i < N; i++)
//   {
//     for (j = 0; j < N; j++)
//     {
//       suma = suma + A[i][j] * A[i][j];
//     }
//   }
//   factor = 1.0 / sqrt(suma);
//   for (i = 0; i < N; i++)
//   {
//     for (j = 0; j < N; j++)
//     {
//       A[i][j] = factor * A[i][j];
//     }
//   }
// }

// #include <stdlib.h>
// #include <stdio.h>
// #include <omp.h>
// void swap(int *x, int *y)
// {
//   int temp = *x;
//   *x = *y;
//   *y = temp;
// }
// int separate(int *x, int low, int high)
// {
//   int i, pivot, last;
//   pivot = x[low];
//   swap(x + low, x + high);
//   last = low;
//   for (i = low; i < high; i++)
//   {
//     if (x[i] <= pivot)
//     {
//       swap(x + last, x + i);
//       last += 1;
//     }
//   }
//   swap(x + last, x + high);
//   return last;
// }
// void qs(int *z, int zstart, int zend, int firstcall)
// {
//   {
//     int pivot;
//     if (firstcall == 1)
//     {
//       qs(z, 0, zend, 0);
//     }
//     else
//     {
//       if (zstart < zend)
//       {
//         pivot = separate(z, zstart, zend);
//         qs(z, zstart, pivot - 1, 0);
//         qs(z, pivot + 1, zend, 0);
//       }
//     }
//   }
// }
// int main(int argc, char **argv)
// {
//   int i, n, *w;
//   n = atoi(argv[1]);
//   w = malloc(n * sizeof(int));
//   for (i = 0; i < n; i++)
//     w[i] = rand() % 100;
//   qs(w, 0, n - 1, 1);
//   if (n < 25)
//     for (i = 0; i < n; i++)
//       printf("%d\n", w[i]);
//   return 0;
// }

double OpArr(double A[], double B[], double C[], int n)
{
  int i, j;
  double s1, s2, a, res;
  suma_de_prefijos(A, n); // obtiene el array de suma de prefijos de A
  quicksort(B, n);        // ordena el array B
  scan_left(C + x, n);    // acumula los valores de elementos de C mas
  // una constante x
  for (i = 0; i < n; i++)
  { /* primer bucle for */
    s1 = 0;
    for (j = 0; j < n; j++)
      s1 += A[j] * B[j];
    for (j = 0; j < n; j++)
      A[j] *= s1;
  }
  for (i = 0; i < n; i++)
  { /* segundo bucle for */
    s2 = 0;
    for (j = 0; j < n; j++)
      s2 += B[j] * C[j];
    for (j = 0; j < n; j++)
      C[j] *= s2;
  }
  /* calculo final */
  a = s1 / s2;
  res = 0;
  for (i = 0; i < n; i++)
    res += a * C[i];
  return res;
}
