#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>
#include <chrono>
#include <boost/program_options.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/operation_blocked.hpp>

//treba programsko nastaviti s skripto
const int DIM = CONST;

namespace po = boost::program_options;
using namespace std;
std::uniform_real_distribution<double> urd;
std::default_random_engine re;

typedef std::vector<std::vector<double>> Tmat;

Tmat newMat(int a, int b){
    std::vector<double> nicelni(a, 0.0);
    Tmat mat(b, nicelni);
    return mat;
}

Tmat transponiraj(Tmat &mat){
    int a = mat.size();
    int b = mat[0].size();
    
    std::vector<double> nicelni1(a, 0.0);
    Tmat mat3(b, nicelni1);
    for (int i = 0; i<a; i+=1){
        for (int j = 0; j<b; j+=1){
            mat3[j][i] = mat[i][j];
        }   
    }
    return mat3;
}

void pomozna_mno_kla(Tmat &mat1, Tmat &mat2, Tmat &mat3,
                     int a1, int a2, 
                     int b1, int b2,
                     int c1, int c2){

    for (int i = a1; i<a2; i+=1){
        for (int j = c1; j<c2; j+=1){
            double s = 0;
            for (int k = b1; k<b2; k+=1){
                    s+=mat1[i][k]*mat2[k][j];
            }
            mat3[i][j] += s;
        }
    }
}


void pomozna_mno_kla_trans(Tmat &mat1, Tmat &mat2, Tmat &mat3,
                     int a1, int a2, 
                     int b1, int b2,
                     int c1, int c2){

    //mat2 je transponirana
    for (int i = a1; i<a2; i+=1){
        for (int j = c1; j<c2; j+=1){
            double s = 0;
            for (int k = b1; k<b2; k+=1){
                    s+=mat1[i][k]*mat2[j][k];
            }
            mat3[i][j] += s;
        }
    }
}


void rek_tra_mno_pomozna(Tmat &mat1, Tmat &mat4, Tmat &mat3,
                         int a1, int a2, 
                         int b1, int b2,
                         int c1, int c2){
    
    int aD = a2-a1;
    int bD = b2-b1;
    int cD = c2-c1;
    int aP = a1+aD/2;
    int bP = b1+bD/2;
    int cP = c1+cD/2;
    
    if(std::min({aD, bD, cD}) < DIM){
        pomozna_mno_kla_trans(mat1, mat4, mat3, a1, a2,  b1, b2, c1, c2);
    }
    else{
        rek_tra_mno_pomozna(mat1, mat4, mat3, a1, aP,  b1, bP, c1, cP);
        rek_tra_mno_pomozna(mat1, mat4, mat3, a1, aP,  b1, bP, cP, c2);
        rek_tra_mno_pomozna(mat1, mat4, mat3, a1, aP,  bP, b2, c1, cP);
        rek_tra_mno_pomozna(mat1, mat4, mat3, a1, aP,  bP, b2, cP, c2);
        
        rek_tra_mno_pomozna(mat1, mat4, mat3, aP, a2,  b1, bP, c1, cP);
        rek_tra_mno_pomozna(mat1, mat4, mat3, aP, a2,  b1, bP, cP, c2);
        rek_tra_mno_pomozna(mat1, mat4, mat3, aP, a2,  bP, b2, c1, cP);
        rek_tra_mno_pomozna(mat1, mat4, mat3, aP, a2,  bP, b2, cP, c2);
    }
}


Tmat MultiplicationRecursiveTransposed(Tmat &mat1, Tmat &mat2){
    Tmat mat4 = transponiraj(mat2);
    
    int a = mat1.size();
    int b = mat1[0].size();
    int c = mat4.size();

    Tmat mat3 = newMat(c,a);
    rek_tra_mno_pomozna(mat1, mat4, mat3, 0, a, 0, b, 0, c);
    
    return mat3;
}

Tmat MultiplicationClassicTransposed(Tmat &mat1, Tmat &mat2){
    Tmat mat4 = transponiraj(mat2);

    int a = mat1.size();
    int b = mat1[0].size();
    int c = mat4.size();
    
    Tmat mat3 = newMat(c,a);
    pomozna_mno_kla_trans(mat1, mat4, mat3, 0, a, 0, b, 0, c);
    return mat3;
}



Tmat RandomMatrix(int m, int n) {
  Tmat mat = newMat(m, n);
  for (int i = 0; i < m; i += 1) {
    for (int j = 0; j < n; j += 1) {
      mat[j][i] = urd(re);
    }
  }
  return mat;
}
void prikaz1(Tmat &mat) {
  for (auto &vrstica : mat) {
    std::cout << "|";
    for (auto &element : vrstica) {
      std::cout << std::setw(10) << element;
    }
    std::cout << "|" << std::endl;
  }
  std::cout << std::endl;
}
boost::numeric::ublas::matrix<double> TmatToBlas(Tmat mat) {
  int m1 = mat.size();
  int n1 = mat[0].size();
  boost::numeric::ublas::matrix<double> bmat(m1, n1);
  for (int i = 0; i < m1; i += 1) {
    for (int j = 0; j < n1; j += 1) {
      bmat(i, j) = mat[i][j];
    }
  }
  return bmat;
}

boost::numeric::ublas::matrix<double> MultiplicationBlas(boost::numeric::ublas::matrix<double> M1,
                                                         boost::numeric::ublas::matrix<double> M2) {
  return boost::numeric::ublas::block_prod<boost::numeric::ublas::matrix<double>, 32>(M1, M2);
}

enum Method {
  CLASSIC, CLASSIC_T, RECURSIVE, RECURSIVE_T, SUBCUBIC, BLAS
};
static map<string, Method> methodMap{
    {"classic",              CLASSIC},
    {"classic_transposed",   CLASSIC_T},
    {"recursive",            RECURSIVE},
    {"recursive_transposed", RECURSIVE_T},
    {"subcubic",             SUBCUBIC},
    {"blas",                 BLAS}
};

int main(int ac, const char **av) {
  int a, b, c;
  int repeat;
  int seed = 2; //totaly random
  int max_time;
  Method method;
  try {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "Display this message")
        ("method,m", po::value<string>()->default_value("subcubic"), "Method to benchmark \n"
            "Available methods:\n"
            "  classic\n"
            "  classic_transposed\n"
            "  recursive\n"
            "  recursive_transposed\n"
            "  subcubic \n"
            "  blas \n"
        )
        ("a,a", po::value<int>()->default_value(512), "A in (A,B)x(B,C) mutiplication  \n ")
        ("b,b", po::value<int>()->default_value(512), "B in (A,B)x(B,C) mutiplication  \n ")
        ("c,c", po::value<int>()->default_value(512), "C in (A,B)x(B,C) mutiplication  \n ")
        ("repeat,r", po::value<int>()->default_value(1), "Number of tests to run")
        ("time,t", po::value<int>()->default_value(0), "Repeat test until arg ms elapsed, overrides --repeat \n ");

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      cout << desc << "\n";
      return 1;
    }

    if (vm.count("method")) {
      string m = vm["method"].as<string>();
      if (methodMap.count(m) > 0) {
        //cout << "testing with method " << m << '\n';
        method = methodMap[m];
      } else {
        cout << m << " is not a valid method, use --help for a list of available methods.";
        return 1;
      }
    } else {
      cout << "method was not set!\n";
      return 1;
    }

    if (vm.count("a")) {
      a = vm["a"].as<int>();
      //cout << "a = " << a << '\n';

    } else {
      cout << "a was not set!\n";
      return 1;
    }
    if (vm.count("b")) {
      b = vm["b"].as<int>();
      //cout << "b = " << b << '\n';

    } else {
      cout << "b was not set!\n";
      return 1;
    }
    if (vm.count("c")) {
      c = vm["c"].as<int>();
      // cout << "c = " << c << '\n';

    } else {
      cout << "c was not set!\n";
      return 1;
    }

    if (vm.count("time")) {
      max_time = vm["time"].as<int>();
    }

    repeat = vm["repeat"].as<int>();
    if (max_time == 0) {
      //cout << "Number of test: " << repeat << endl;
    } else {
      //cout << "Testing for " << max_time << " ms" << endl;
    }

  } catch (std::exception &e) {
    cerr << e.what();
  }


  //Execute the tests
  urd = std::uniform_real_distribution<double>(0, 1);
  re = std::default_random_engine(seed);
  Tmat m1 = RandomMatrix(b, a);
  Tmat m2 = RandomMatrix(c, b);
  boost::numeric::ublas::matrix<double> bm1, bm2;

  std::function<Tmat(Tmat &, Tmat &)> f;
  auto blas_f = MultiplicationBlas;

  switch (method) {
    //case CLASSIC:f = MultiplicationClassic;
    //  break;
    case CLASSIC_T:f = MultiplicationClassicTransposed;
      break;
    //case RECURSIVE:f = MultiplicationRecursive;
    //  break;
    case RECURSIVE_T:f = MultiplicationRecursiveTransposed;
    //  break;
    //case SUBCUBIC:f = MultiplicationSubcubic;
      break;
    case BLAS :bm1 = TmatToBlas(m1);
      bm2 = TmatToBlas(m2);
  }


  std::chrono::time_point<std::chrono::steady_clock> time_start, time_end;
  time_start = std::chrono::steady_clock::now();
  if (method == BLAS) {
    for (int i = 0; i < repeat; i+=1) {
      auto bm3 = blas_f(bm1, bm2);
    }
  } else {
    for (int i = 0; i < repeat; i+=1) {
      Tmat m3 = f(m1, m2);
    }
  }

  time_end = std::chrono::steady_clock::now();
  auto time_total = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
  std::cout << time_total.count() / repeat << endl;
}
