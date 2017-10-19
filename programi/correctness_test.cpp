#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include <iomanip>
#include <cmath>
#include "matrix.h"

Tmat PsevdoId(int m, int n) {
  Tmat mat(m, n);
  for (int k = 0; k < std::min({m, n}); k += 1) {
    mat(k, k) = 1;
  }
  return mat;
}

Tmat Ex1Matrix(int m, int n) {
  Tmat mat(m, n);
  for (int i = 0; i < m; i += 1) {
    for (int j = 0; j < n; j += 1) {
      mat(i, j) = 1 / (1 + i + j);
    }
  }
  return mat;
}

Tmat Ex2Matrix(int m, int n) {
  Tmat mat(m, n);
  for (int i = 0; i < m; i += 1) {
    for (int j = 0; j < n; j += 1) {
      mat(i, j) = i + j;
    }
  }
  return mat;
}

Tmat Ex3Matrix(int m, int n) {
  Tmat mat(m, n);
  for (int i = 0; i < m; i += 1) {
    mat(i, 0) = 1;
  }
  return mat;
}

Tmat Ex4Matrix(int m, int n) {
  Tmat mat(m, n);
  for (int j = 0; j < n; j += 1) {
    mat(0, j) = 1;
  }
  return mat;
}

Tmat OnesMatrix(int m, int n) {
  Tmat mat(m, n);
  for (int i = 0; i < m; i += 1) {
    for (int j = 0; j < n; j += 1) {
      mat(i, j) = 1;
    }
  }
  return mat;
}

Tmat MatrixDiference(Tmat mat1, Tmat mat2) {
  int m = mat1.m;
  int n = mat1.n;
  Tmat mat(m, n);
  for (int i = 0; i < m; i += 1) {
    for (int j = 0; j < n; j += 1) {
      mat(i, j) = mat1(i, j) - mat2(i, j);
    }
  }
  return mat;
}

bool ZeroMatrix(Tmat mat) {
  int m = mat.m;
  int n = mat.n;
  double eps = 0.000001;
  for (int i = 0; i < m; i += 1) {
    for (int j = 0; j < n; j += 1) {
      if (mat(i,j) > eps) {
        std::cout << "i,j,mat(i,j) " << i << " " << j << " " << mat(i,j) << std::endl;
        return false;
      }
    }
  }
  return true;
}

void TestCorrectness(std::function<Tmat(Tmat &, Tmat &)> TestAlg, int a, int b, int c) {
  double eps = 0.001;
  std::cout << "Testing correctess of algorithm." << std::endl;
  std::cout << "For matrix of size: a = " << a << " b = " << b << " c = " << c << std::endl;
  
  Tmat mat1 = PsevdoId(a,b);
  Tmat mat2 = PsevdoId(b,c);
  Tmat mat3 = MultiplicationClassicTransposed(mat1, mat2);
  
  for (int i = 0; i < mat3.m; i += 1) {
    for (int j = 0; j < mat3.n; j += 1) {
      if (i == j){
        if (std::abs(mat3(i,j) - 1) > eps){
          std::cout << "Critical error: Multiplication classic transposed failed." << std::endl;
          throw;
        }
      } else{
        if (mat3(i,j) != 0){
          std::cout << "Critical error: Multiplication classic transposed failed." << std::endl;
          throw;
        }
      }
    }
  }
  
  mat1 = Ex1Matrix(a,b);
  mat2 = Ex2Matrix(b,c);
  mat3 = MultiplicationClassicTransposed(mat1, mat2);
  Tmat mat4 = TestAlg(mat1, mat2);
  Tmat diff = MatrixDiference(mat3, mat4);
  
  if (ZeroMatrix(diff)) {
    std::cout << "Static test ok." << std::endl;
  } else {
    std::cout << "Static test failed." << std::endl;
  }
  
  mat1 = RandomMatrix(a,b);
  mat2 = RandomMatrix(b,c);
  mat3 = MultiplicationClassicTransposed(mat1, mat2);
  mat4 = TestAlg(mat1, mat2);
  diff = MatrixDiference(mat3, mat4);
  
  if (ZeroMatrix(diff)) {
    std::cout << "Random test ok." << std::endl;
  } else {
    std::cout << "Random test failed." << std::endl;
  }
  
  //PrintMatrix(mat1);
  //PrintMatrix(mat2);
  //PrintMatrix(mat3);
  //PrintMatrix(mat4);
}
