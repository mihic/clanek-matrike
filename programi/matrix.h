#include <functional>

//typedef std::vector<std::vector<double>> Tmat;

class Tmat {
 private:
  double *mat;
 public:
  int m;
  int n;
  Tmat(int a, int b) {
    m = a;
    n = b;
    mat = new double[a * b]();
  }
  ~Tmat() {
    //delete mat;
  }
  inline double operator()(int a, int b) const {
    return mat[n * a + b];
  }
  inline double &operator()(int a, int b) {
    return mat[n * a + b];
  }
};

Tmat transponiraj(Tmat &mat);
Tmat MultiplicationClassicTransposed(Tmat &, Tmat &);
Tmat MultiplicationClassic(Tmat &, Tmat &);
Tmat MultiplicationRecursive(Tmat &, Tmat &);
Tmat MultiplicationRecursiveTransposed(Tmat &, Tmat &);
Tmat MultiplicationSubcubic(Tmat &, Tmat &);
void rek_tra_mno_pomozna(Tmat &mat1, Tmat &mat4, Tmat &mat3,
                         int a1, int a2,
                         int b1, int b2,
                         int c1, int c2);
void pomozna_mno_kla(Tmat &mat1, Tmat &mat2, Tmat &mat3,
                     int a1, int a2,
                     int b1, int b2,
                     int c1, int c2);
//Tmat newMat(int a, int b);
void pomozna_mno_kla_trans(Tmat &mat1, Tmat &mat2, Tmat &mat3,
                           int a1, int a2,
                           int b1, int b2,
                           int c1, int c2);
Tmat strassen_mul(Tmat &, Tmat &);

void rek_mno_pomozna(Tmat &mat1, Tmat &mat2, Tmat &mat3,
                     int a1, int a2,
                     int b1, int b2,
                     int c1, int c2);
Tmat pomozno_sestevanje(Tmat &mat1, Tmat &mat2,
                        int x1, int x1max,
                        int x2, int x2max,
                        int y1, int y1max,
                        int y2, int y2max);
Tmat vrni_obmocje_matrike(Tmat &mat1,
                          int x1, int x1max,
                          int x2, int x2max);
Tmat pomozno_odstevanje(Tmat &mat1, Tmat &mat2,
                        int x1, int x1max,
                        int x2, int x2max,
                        int y1, int y1max,
                        int y2, int y2max);
void pristej_rezultatu(Tmat &mat1, Tmat &mat3,
                       int x1,
                       int x2);
void odstej_rezultatu(Tmat &mat1, Tmat &mat3,
                      int x1,
                      int x2);

void TestCorrectness(std::function<Tmat(Tmat &, Tmat &)>, int, int, int);

void prikaz(Tmat &mat);

Tmat RandomMatrix(int, int);
