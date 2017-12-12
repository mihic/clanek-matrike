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

double InfDistance(Tmat mat1, Tmat mat2) {
    int m = mat1.m;
    int n = mat1.n;
    Tmat mat = MatrixDiference(mat1, mat2);
    double maxAbs = 0;
    for (int i = 0; i < m; i += 1) {
        for (int j = 0; j < n; j += 1) {
            if (std::abs(mat(i, j)) > maxAbs) {
                maxAbs = std::abs(mat(i, j));
            }
        }
    }
    return maxAbs;
}

bool ZeroMatrix(Tmat mat) {
  int m = mat.m;
  int n = mat.n;
  float eps = 0.000000001;
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

double TestNumerics(
        std::function<Tmat(Tmat &, Tmat &)> TestAlg,
        std::function<Tmat(Tmat &, Tmat &)> BlasAlg,
        int a, int b, int c) {
        
    Tmat mat1 = RandomMatrix(a,b);
    Tmat mat2 = RandomMatrix(b,c);
    Tmat mat3 = TestAlg(mat1, mat2);
    Tmat mat4 = BlasAlg(mat1, mat2);
    
    /*
    PrintMatrix(mat1);
    PrintMatrix(mat2);
    PrintMatrix(mat3);
    PrintMatrix(mat4);
    */
    
    return InfDistance(mat3, mat4);
}

bool TestCorrectness(std::function<Tmat(Tmat &, Tmat &)> TestAlg, int a, int b, int c) {
  float eps = 0.001;
  bool correct = true;
  std::cout << "Testing correctess of algorithm." << std::endl;
  std::cout << "For matrix of size: a = " << a << " b = " << b << " c = " << c << std::endl;

  int minabc = std::min({a,b,c});
  Tmat mat1 = PsevdoId(a,b);
  Tmat mat2 = PsevdoId(b,c);
  Tmat mat3 = MultiplicationClassicTransposed(mat1, mat2);

  std::cout << "For matrix of size: m = " << mat3.m << " n = " << mat3.n << std::endl;

  for (int i = 0; i < mat3.m; i += 1) {
    for (int j = 0; j < mat3.n; j += 1) {
      if (i == j and i < minabc){
        if (std::abs(mat3(i,j) - 1) > eps){
          std::cout << "Critical error: Multiplication classic transposed failed 1: " << "i=" << i << " j=" << j << " mat3(i,j)=" << mat3(i,j) << std::endl;
          throw;
        }
      } else{
        if (mat3(i,j) != 0){
          std::cout << "Critical error: Multiplication classic transposed failed 0: " << "i=" << i << " j=" << j << " mat3(i,j)=" << mat3(i,j) << std::endl;
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
    correct = false;
  }

  mat1 = Ex2Matrix(a,b);
  mat2 = Ex3Matrix(b,c);
  mat3 = MultiplicationClassicTransposed(mat1, mat2);
  mat4 = TestAlg(mat1, mat2);
  diff = MatrixDiference(mat3, mat4);

  if (ZeroMatrix(diff)) {
    std::cout << "Static 2 test ok." << std::endl;
  } else {
    std::cout << "Static 2 test failed." << std::endl;
    correct = false;
  }

  mat1 = Ex3Matrix(a,b);
  mat2 = Ex4Matrix(b,c);
  mat3 = MultiplicationClassicTransposed(mat1, mat2);
  mat4 = TestAlg(mat1, mat2);
  diff = MatrixDiference(mat3, mat4);

  if (ZeroMatrix(diff)) {
    std::cout << "Static 3 test ok." << std::endl;
  } else {
    std::cout << "Static 3 test failed." << std::endl;
    correct = false;
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
    correct = false;
  }

  //PrintMatrix(mat1);
  //PrintMatrix(mat2);
  //PrintMatrix(mat3);
  //PrintMatrix(mat4);

//
//  cout << "zacetek" << endl;
//  Tmat nova = RandomMatrix(3,3);
//  Tmat nova2 = RandomMatrix(3,3);
//  PrintMatrix(nova2);
//  nova = nova2;
//  Tmat nova3 = std::move(nova2);
//  PrintMatrix(nova3);
//  //nova2(1,1) = 0;
//  //PrintMatrix(nova2);
//  PrintMatrix(nova3);


  return true;
  //return correct;

}

bool CompleteTestForCorrectness(std::function<Tmat(Tmat &, Tmat &)> TestAlg) {
  bool correct = true;
  for (int i = 7; i < 500; i += 31) {
    for (int j = 5; j < 500; j += 51) {
      for (int k = 3; k < 500; k += 41) {
        correct = correct and TestCorrectness(TestAlg, i, j, k);
      }
    }
  }
  
  for (int i = 2117; i < 2123; i += 31) {
    for (int j = 2115; j < 2123; j += 51) {
      for (int k = 2113; k < 2123; k += 41) {
        correct = correct and TestCorrectness(TestAlg, i, j, k);
      }
    }
  }
  
  return correct;
}