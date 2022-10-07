#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include <cmath>
#include <iostream>
#include <algorithm>
#include <string.h>

// int N = 106;
// int THREADS = 8;
int N, THREADS;


void swap(double *x, double *y)
{
  double temp = *x;
  *x = *y;
  *y = temp;
}

int separate(double *x, int low, int high)
{
  int i, last;
  double pivot;
  pivot = x[low];
  swap(x + low, x + high);
  last = low;
  for (i = low; i < high; i++)
  {
    if (x[i] <= pivot)
    {
      swap(x + last, x + i);
      last += 1;
    }
  }
  swap(x + last, x + high);
  return last;
}

void qs(double *z, int zstart, int zend)
{
  // printf("hilo en qs: %i, nivel: %i, size: %i\n",
  //        omp_get_thread_num(), omp_get_active_level(), (zend - zstart));
  int pivot;
  {
    {
      if (zstart < zend)
      {
        pivot = separate(z, zstart, zend);

      #pragma omp task shared(z) if ((zend - zstart) > N / THREADS)
        qs(z, zstart, pivot - 1);

      #pragma omp task shared(z) if ((zend - zstart) > N / THREADS)
        qs(z, pivot + 1, zend);

    #pragma omp taskwait
      }
    }
  }
}

void pqs(double *z, int zend)
{
#pragma omp parallel
  {
#pragma omp single
    {
      qs(z, 0, zend - 1);
      // printf("finished pqs in %i\n",omp_get_thread_num());
    }
  }
}

void suma_de_prefijos(double *A, int n)
{
  double og_last = A[n-1]; 

  // sweep up
  long dmax = log2(n - 1);
  for (long d = 0; d <= dmax; d++)
  {
  #pragma omp parallel for
    for (long long k = 0; k < n; k += (1 << (d + 1)))
    {
      A[k + (1 << (d + 1)) - 1] = A[k + (1 << d) - 1] + A[k + (1 << (d + 1)) - 1];
    }
  }

  // printf("sweep up finished:\n");
  // for (int i = 0; i < n; i++)
  // {
  //   printf("%f,", A[i]);
  // }
  // printf("\n");
  
  // sweep down
  A[n - 1] = 0;
  for (long d = dmax; d >= 0; d--)
  {
  #pragma omp parallel for
    for (long long k = 0; k < n; k += (1 << (d + 1)))
    {
      auto temp = A[k + (1 << d) - 1];
      A[k + (1 << d) - 1] = A[k + (1 << (d + 1)) - 1];
      A[k + (1 << (d + 1)) - 1] += temp;
    }
  }

  // printf("sweep down finished:\n");
  // for (int i = 0; i < n; i++)
  // {
  //   printf("%f,", A[i]);
  // }
  // printf("\n");
  
  for (int i = 0; i != sizeof(A) - 1; i++) {
   *(A + i) = *(A + i + 1);
  }
  A[n-1] = A[n-1] + og_last;

  // printf("shifted array:\n");
  // for (int i = 0; i < n; i++)
  // {
  //   printf("%f,", A[i]);
  // }
  // printf("\n");
  // printf("finished prefix_sum in %i\n",omp_get_thread_num());

}

void scan_left(double *C, int n, double &x)
{
  // printf("before scan:\n");
  // for (int i = 0; i < n; i++)
  // {
  //   printf("%f,", C[i]);
  // }
  // printf("\n");

  long dmax = log2(n - 1);
  for (long d = 0; d <= dmax; d++)
  {
  #pragma omp parallel for
    for (long long k = 0; k < n; k += (1 << (d + 1)))
    {
      C[k + (1 << (d + 1)) - 1] = C[k + (1 << d) - 1] + C[k + (1 << (d + 1)) - 1];
    }
  }

  // printf("after scan:\n");
  // for (int i = 0; i < n; i++)
  // {
  //   printf("%f,", C[i]);
  // }
  // printf("\n");

  x += C[n - 1];
  // printf("finished scan_left in %i\n",omp_get_thread_num());

}

double multMod(double *A, double *B, int n)
{
  // printf("hilo en multmod: %i\n",
  //       omp_get_thread_num());

  double s1;
  for (int i = 0; i < n; i++)
  { 
    s1 = 0;
  #pragma omp parallel for reduction(+ \
                                   : s1)
    for (int j = 0; j < n; j++){
      s1 += (log10(A[j] * B[j])); 
    }
    // barrera implicita, no se pueden paralelizar ambos fors 
    // debido a necesitar s1 completo.
    // printf("s_in:%f, it: %d\n",s1 , i);

  #pragma omp parallel for
    for (int j = 0; j < n; j++)
      A[j] += (log10(s1));
  }

  return s1;
}

double OpArr(double A[], double B[], double C[], int n)
{
  int i, j;
  double s1, s2, a, res;
  double x = 1.0;
  #pragma omp parallel
  {
    #pragma omp single nowait
    {
      #pragma omp task depend(out:A)
        suma_de_prefijos(A, n); // obtiene el array de suma de prefijos de A
      #pragma omp task depend(out:B)
        pqs(B, n); // ordena el array B
      #pragma omp task depend(out:C)
        scan_left(C, n, x); // acumula los valores de elementos de C mas una constante x

      #pragma omp task depend(in:A,B)
        s1 = multMod(A, B, n);
      #pragma omp task depend(in:C,B)
        s2 = multMod(C, B, n);
    }
  }
  /* calculo final */
  // printf("s1:%f\ns2:%f\n", s1,s2);

  a = s1 / s2;

  // printf("a:%f\n",a);

  res = 0;
  #pragma omp parallel for reduction(+ \
                                    : res)
  for (int i = 0; i < n; i++){
    res += a + C[i];
  }
  return res;
}

int main()
{
  std::cin >> N >> THREADS;
  
  omp_set_num_threads(THREADS);
  omp_set_nested(1);
  omp_set_dynamic(0);
  double t1, t2;

  // const int n = 1 << 14;
  const int n = N;

  double a[n], b[n], c[n];

  for (int i = 0; i < n; i++)
  {
    a[i] = rand() % 100;
    b[i] = rand() % 100;
    c[i] = rand() % 100;
  }

  t1 = omp_get_wtime();
  double result = OpArr(a, b, c, n);  
  t2 = omp_get_wtime();

  printf("result: %f\n", result);
  printf("%f8", (t2 - t1));

  // for (int i = 0; i < n; i++)
  // {
  //   printf("%f,", a[i]);
  // }
  // printf("\n");


  // // suma_de_prefijos(a,n);
  // double equis = 5.0;
  // scan_left(c,n,equis);

  
  // for (int i = 0; i < n; i++)
  // {
  //   printf("%f,", a[i]);
  // }
  return 0;
}