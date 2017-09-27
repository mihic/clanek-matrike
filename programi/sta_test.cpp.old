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
  Tmat mat = newMat(m, n);
  for (int k = 0; k < std::min({m,n}); k += 1) {
    mat[k][k] = 1;
  }
  return mat;
}

Tmat Ex1Matrix(int m, int n) {
  Tmat mat = newMat(m, n);
  for (int i = 0; i < m; i += 1) {
    for (int j = 0; j < n; j += 1) {
      mat[j][i] = 1/(1+i+j);
    }
  }
  return mat;
}

Tmat Ex2Matrix(int m, int n) {
  Tmat mat = newMat(m, n);
  for (int i = 0; i < m; i += 1) {
    for (int j = 0; j < n; j += 1) {
      mat[j][i] = i+j;
    }
  }
  return mat;
}

Tmat Ex3Matrix(int m, int n) {
  Tmat mat = newMat(m, n);
  for (int i = 0; i < m; i += 1) {
    mat[0][i] = 1;
  }
  return mat;
}

Tmat Ex4Matrix(int m, int n) {
  Tmat mat = newMat(m, n);
  for (int j = 0; j < n; j += 1) {
    mat[j][0] = 1;
  }
  return mat;
}

Tmat OnesMatrix(int m, int n) {
  Tmat mat = newMat(m, n);
  for (int i = 0; i < m; i += 1) {
    for (int j = 0; j < n; j += 1) {
      mat[j][i] = 1;
    }
  }
  return mat;
}

Tmat MatrixDiference(Tmat mat1, Tmat mat2) {
  //std::cout << "Testing3.21" << std::endl;
  int m = mat1.size();
  int n = mat1[0].size();
  Tmat mat = newMat(n, m);
  //std::cout << mat.size() << std::endl;
  //std::cout << mat[0].size() << std::endl;
  //std::cout << "Testing3.23" << std::endl;
  for (int i = 0; i < n; i += 1) {
    for (int j = 0; j < m; j += 1) {
      mat[j][i] = mat1[j][i] - mat2[j][i];
    }
  }
  return mat;
}

bool ZeroMatrix(Tmat mat){
  int m = mat.size();
  int n = mat[0].size();
  //double eps = 0.000001;
  double eps = 0.1;
  for (int i = 0; i < n; i += 1) {
    for (int j = 0; j < m; j += 1) {
      if (mat[j][i] > eps) {
        return false;
      }
    }
  }
  return true;
}

//tu ne damo linka na matrike ampak jih skopiramo
//ce je slucajno algoritem ki spreminja vhod
void TestCorrectness(std::function<Tmat(Tmat &, Tmat &)> TestAlg) {
    std::cout << "Testing" << std::endl;
    
    Tmat mat1;
    Tmat mat2;
    Tmat mat3;
    Tmat mat4;
    
    int n = 1024;
    int a = 2323;
    int b = 1333;
    int c = 1007;
    
    mat1 = RandomMatrix(b,a);
    mat2 = RandomMatrix(c,b);
    
    //mat1 = RandomMatrix(n,n);
    //mat2 = RandomMatrix(n,n);
    
    std::cout << "Testing1" << std::endl;
    mat3 = MultiplicationClassicTransposed(mat1, mat2);
    std::cout << "Testing2" << std::endl;
    mat4 = TestAlg(mat1, mat2);
    std::cout << "Testing3" << std::endl;
    
    std::cout << mat3.size() << " # " << mat3[0].size() << std::endl;
    std::cout << mat4.size() << " # " << mat4[0].size() << std::endl;
    
    //std::cout << "Testing3.2" << std::endl;
    MatrixDiference(mat3,mat4);
    
    //std::cout << "Testing3.5" << std::endl;
    
    if (ZeroMatrix(MatrixDiference(mat3,mat4))) {
      std::cout << "OK" << std::endl;
      //return ;
    } else {
      std::cout << "False" << std::endl;
    }
    
    //n = 16;
    a = 11;
    b = 13;
    c = 12;
    
    std::cout << "Testing4" << std::endl;
    //mat1 = Ex2Matrix(b,a);
    //mat2 = Ex3Matrix(c,b);
    //mat1 = Ex3Matrix(b,a);
    //mat2 = Ex2Matrix(c,b);
    
    mat1 = RandomMatrix(b,a);
    mat2 = RandomMatrix(c,b);
    mat3 = MultiplicationClassicTransposed(mat1, mat2);
    mat4 = TestAlg(mat1, mat2);
    prikaz(mat3);
    prikaz(mat4);
    std::cout << "Testing5" << std::endl;
    
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
    prikaz(mat4);
    */
}
