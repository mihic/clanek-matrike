#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <random>
#include <iomanip>
#include <chrono>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/operation_blocked.hpp>
#include "matrix.h"

namespace po = boost::program_options;
using namespace std;
std::uniform_real_distribution<double> urd;
std::default_random_engine re;

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
  int seed = 1; //totaly random
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
      cout << "Number of test: " << repeat << endl;
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
    case CLASSIC:f = MultiplicationClassic;
      break;
    case CLASSIC_T:f = MultiplicationClassicTransposed;
      break;
    case RECURSIVE:f = MultiplicationRecursive;
      break;
    case RECURSIVE_T:f = MultiplicationRecursiveTransposed;
      break;
    case SUBCUBIC:f = MultiplicationSubcubic;
      break;
    case BLAS :bm1 = TmatToBlas(m1);
      bm2 = TmatToBlas(m2);
  }

  if (max_time > 0) {
    int count = 0;
    std::chrono::time_point<std::chrono::steady_clock> time_start, time_end;
    auto time_total = std::chrono::milliseconds{0};
    while (true) {
      time_start = std::chrono::steady_clock::now();
      if (method == BLAS) {
        auto bm3 = blas_f(bm1, bm2);
      } else {
        Tmat m3 = f(m1, m2);
      }
      time_end = std::chrono::steady_clock::now();
      auto time_of_test = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
      //cout << time_of_test.count() << endl;
      time_total += time_of_test;
      count++;
      if (time_total.count() > max_time || count > 10000) {
        //cout << "Total time:" << time_total.count() << "ms" << endl;
        //cout << "Iteration time:" << time_total.count() / count << "ms" << endl;
        cout << time_total.count() / count << endl;
        break;
      }
    }
  } else {

    std::chrono::time_point<std::chrono::steady_clock> time_start, time_end;
    time_start = std::chrono::steady_clock::now();
    if (method == BLAS) {
      for (int i = 0; i < repeat; ++i) {
        auto bm3 = blas_f(bm1, bm2);
      }
    } else {
      for (int i = 0; i < repeat; ++i) {
        Tmat m3 = f(m1, m2);
      }
    }

    time_end = std::chrono::steady_clock::now();
    auto time_total = std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
    std::cout << "Total time:" << time_total.count() << " ms\n";
    std::cout << "Time per iteration:" << time_total.count() / repeat << " ms\n";

  }
}


