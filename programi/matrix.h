#include <functional>
#include <iostream>
#include <memory>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

class Tmat {
 private:
  std::unique_ptr<double[]> mat;
 public:
  int m;
  int n;

  //constructor
  Tmat(int a, int b) : m(a), n(b), mat(new double[a * b]()) {
  }

  //copy constructor
  Tmat(const Tmat& other) : m(other.m), n(other.n),
                            mat(new double[other.n * other.m]) {
    std::copy_n(other.mat.get(),m*n,mat.get());
  }

  //copy assignment
  Tmat& operator=(const Tmat& other) {
    if (&other == this) return *this;
    m = other.m;
    n = other.n;
    mat.reset(new double[m*n]);
    std::copy_n(other.mat.get(),m*n,mat.get());
    return *this;
  }


  //move constructor
  Tmat(Tmat&& other) : m(other.m), n(other.n) {
    mat = std::move(other.mat);
  }
  //move assign
  Tmat& operator=(Tmat&& other) {
    m = other.m;
    n = other.n;
    mat = std::move(other.mat);
    return *this;
  }


  inline double operator()(int a, int b) const {
    return mat.get()[n * a + b];
  }
  inline double &operator()(int a, int b) {
    return mat.get()[n * a + b];
  }
};

//class Tmat {
// private:
//  double* mat;
// public:
//  int m;
//  int n;
//
//  //constructor
//  Tmat(int a, int b) : m(a), n(b), mat(new double[a * b]()) {
//  }
//
//  //copy constructor
//  Tmat(const Tmat& other) : m(other.m), n(other.n),
//                            mat(new double[other.n * other.m]) {
//    std::copy_n(other.mat,m*n,mat);
//  }
//
//  //copy assignment
//  Tmat& operator=(const Tmat& other) {
//    if (&other == this) return *this;
//    m = other.m;
//    n = other.n;
//    delete[] mat;
//    mat = new double[n * m];
//    std::memcpy(mat, other.mat, sizeof(double) * n * m);
//    return *this;
//  }
//
//
//  //move constructor
//  Tmat(Tmat&& other) : m(other.m), n(other.n), mat(other.mat) {
//    other.mat = nullptr;
//  }
//  //move assign
//  Tmat& operator=(Tmat&& other) {
//    m = other.m;
//    n = other.n;
//    delete[] mat;
//    mat = other.mat;
//    other.mat = nullptr;
//    return *this;
//  }
//
//  //destructor
//  ~Tmat() {
//    delete[] mat;
//  }
//
//  inline double operator()(int a, int b) const {
//    return mat[n * a + b];
//  }
//  inline double &operator()(int a, int b) {
//    return mat[n * a + b];
//  }
//};



//class Tmat {
// private:
//  std::vector<std::vector<double>> mat;
// public:
//  int m;
//  int n;
//  Tmat(int a, int b) {
//    m = a;
//    n = b;
//    std::vector<double> nicelni(n, 0.0);
//    std::vector<std::vector<double>> matX(m, nicelni);
//    mat = matX;
//  }
//
//  ~Tmat(){
//
//  }
//
//  inline double operator()(int a, int b) const {
//    return mat.at(a).at(b);
//  }
//  inline double &operator()(int a, int b) {
//    return mat.at(a).at(b);
//  }
//};


Tmat Transpose(Tmat &mat);
Tmat MultiplicationClassicTransposed(Tmat &, Tmat &);
Tmat MultiplicationClassic(Tmat &, Tmat &);
Tmat MultiplicationRecursive(Tmat &, Tmat &);
Tmat MultiplicationRecursiveTransposed(Tmat &, Tmat &);
Tmat MultiplicationStrassenTransposed(Tmat &, Tmat &);
void MultiplicationRecursiveTransposedPart(Tmat &mat1, Tmat &mat4, Tmat &mat3,
                                           int a1, int a2,
                                           int b1, int b2,
                                           int c1, int c2);
void MultiplicationClassicPart(Tmat &mat1, Tmat &mat2, Tmat &mat3,
                               int a1, int a2,
                               int b1, int b2,
                               int c1, int c2);
//Tmat newMat(int a, int b);
void MultiplicationClassicTransposedPart(Tmat &mat1, Tmat &mat2, Tmat &mat3,
                                         int a1, int a2,
                                         int b1, int b2,
                                         int c1, int c2);
Tmat MultiplicationStrassen(Tmat &, Tmat &);

void MultiplicationRecursivePart(Tmat &mat1, Tmat &mat2, Tmat &mat3,
                                 int a1, int a2,
                                 int b1, int b2,
                                 int c1, int c2);
Tmat Add(Tmat &mat1, Tmat &mat2,
         int x1, int x1max,
         int x2, int x2max,
         int y1, int y1max,
         int y2, int y2max);
Tmat Slice(Tmat &mat1,
           int x1, int x1max,
           int x2, int x2max);
Tmat Sub(Tmat &mat1, Tmat &mat2,
         int x1, int x1max,
         int x2, int x2max,
         int y1, int y1max,
         int y2, int y2max);
void AddToResult(Tmat &mat1, Tmat &mat3,
                 int x1,
                 int x2);
void SubFromResult(Tmat &mat1, Tmat &mat3,
                   int x1,
                   int x2);

bool TestCorrectness(std::function<Tmat(Tmat &, Tmat &)>, int, int, int);

bool CompleteTestForCorrectness(std::function<Tmat(Tmat &, Tmat &)>);

void PrintMatrix(Tmat &mat);

Tmat RandomMatrix(int, int);

bool DimensionCheckFailed(Tmat &mat1, Tmat &mat2);
