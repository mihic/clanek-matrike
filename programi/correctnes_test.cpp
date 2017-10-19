#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include <iomanip>
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
  //std::cout << "Testing3.21" << std::endl;
  int m = mat1.m;
  int n = mat1.n;
  Tmat mat(m, n);
  //std::cout << mat.size() << std::endl;
  //std::cout << mat[0].size() << std::endl;
  //std::cout << "Testing3.23" << std::endl;
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
  //double eps = 0.000001;
  double eps = 0.1;
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

//tu ne damo linka na matrike ampak jih skopiramo
//ce je slucajno algoritem ki spreminja vhod
void TestCorrectness(std::function<Tmat(Tmat &, Tmat &)> TestAlg, int a, int b, int c) {
  std::cout << "Testing" << std::endl;
  /*
  int n = 1024;
  int a = 512;
  int b = 1024;
  int c = 512;
  */
  int n = 10;
  //int a = 19;
  //int b = 17;
  //int c = 12;
  
  Tmat mat1 = Ex2Matrix(a,b);
  Tmat mat2 = Ex3Matrix(b,c);

  //mat1 = RandomMatrix(n,n);
  //mat2 = RandomMatrix(n,n);

  std::cout << "Testing1" << std::endl;
  Tmat mat3 = MultiplicationClassic(mat1, mat2);
  std::cout << "Testing2" << std::endl;
  Tmat mat4 = TestAlg(mat1, mat2);
  std::cout << "Testing3" << std::endl;

  std::cout << mat3.n << " # " << mat3.m << std::endl;
  std::cout << mat4.n << " # " << mat4.m << std::endl;

  std::cout << "Testing3.2" << std::endl;
  Tmat diff = MatrixDiference(mat3, mat4);
//  PrintMatrix(mat1);
  //PrintMatrix(mat2);
//  PrintMatrix(mat3);
//  PrintMatrix(mat4);
  PrintMatrix(diff);
  std::cout << "Testing3.5" << std::endl;

  std::cout << "DDD" << std::endl;
  if (ZeroMatrix(diff)) {
    std::cout << "OK" << std::endl;
    std::cout << "AAA"  << std::endl;
    //return ;
  } else {
    std::cout << "False" << std::endl;
    std::cout << "BBB" << std::endl;
  }
  
  std::cout << "CCC" << std::endl;
//
//    //n = 16;
//    a = 11;
//    b = 13;
//    c = 12;
//
//    std::cout << "Testing4" << std::endl;
//    //mat1 = Ex2Matrix(b,a);
//    //mat2 = Ex3Matrix(c,b);
//    //mat1 = Ex3Matrix(b,a);
//    //mat2 = Ex2Matrix(c,b);
//
//    mat1 = RandomMatrix(b,a);
//    mat2 = RandomMatrix(c,b);
//    mat3 = MultiplicationClassicTransposed(mat1, mat2);
//    mat4 = TestAlg(mat1, mat2);
//    //PrintMatrix(mat3);
//    //PrintMatrix(mat4);
//    std::cout << "Testing5" << std::endl;

  /*
  int a = 800;
  int b = 600;
  int c = 400;
  */

/*
    mat1 = Ex2Matrix(b,a);
    mat2 = PsevdoId(c,b);
    mat3 = MultiplicationClassicTransposed(mat1, mat2);
    mat4 = TestAlg(mat1, mat2);

    prikaz(mat1);
    prikaz(mat2); 
    prikaz(mat3);
    prikaz(mat4);
    /*
    mat1 = Ex2Matrix(b,a);
    mat2 = Ex4Matrix(c,b);
    mat3 = MultiplicationClassicTransposed(mat1, mat2);
    mat4 = TestAlg(mat1, mat2);
 
    prikaz(mat3);
    prikaz(mat4);
        
    mat1 = Ex4Matrix(b,a);
    mat2 = Ex2Matrix(c,b);
    mat3 = MultiplicationClassicTransposed(mat1, mat2);
    mat4 = TestAlg(mat1, mat2);
 
    prikaz(mat3);
    PrintMatrix(mat4);
    */
    
    std::cout << "Izvede se ok." << std::endl;
}
