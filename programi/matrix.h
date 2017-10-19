#include <functional>
#include <iostream>
//typedef std::vector<std::vector<double>> Tmat;

class Tmat {
 private:
  std::vector<double> mat;
 public:
  int m;
  int n;
  Tmat(int a, int b) {
    m = a;
    n = b;
    //mat = new double[a * b]();
    mat = std::vector<double>(a * b);
  }
  inline double operator()(int a, int b) const {
    return mat[n * a + b];
  }
  inline double &operator()(int a, int b) {
    return mat[n * a + b];
  }
};

Tmat Transpose(Tmat &mat);
Tmat MultiplicationClassicTransposed(Tmat &, Tmat &);
Tmat MultiplicationClassic(Tmat &, Tmat &);
Tmat MultiplicationRecursive(Tmat &, Tmat &);
Tmat MultiplicationRecursiveTransposed(Tmat &, Tmat &);
Tmat MultiplicationSubcubic(Tmat &, Tmat &);
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

void TestCorrectness(std::function<Tmat(Tmat &, Tmat &)>, int, int, int);

void PrintMatrix(Tmat &mat);

Tmat RandomMatrix(int, int);
