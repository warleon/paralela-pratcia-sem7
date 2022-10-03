#include <omp.h>

#include <cmath>
#include <iostream>

#define N 8

void normaliza(double A[N][N]) {
  double suma = 0.0, factor;
#pragma omp parallel for reduction(+ : suma)
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      suma += A[i][j] * A[i][j];
    }
  }
  factor = 1.0 / sqrt(suma);
#pragma omp parallel for
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      A[i][j] = factor * A[i][j];
    }
  }
}

int main() {
  double A[N][N];
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      A[i][j] = (double)rand();
    }
  }

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      std::cout << A[i][j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout
      << "----------------------------------------------------------------\n";
  normaliza(A);
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      std::cout << A[i][j] << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}