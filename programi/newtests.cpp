#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <random>
#include <iomanip>
#include <chrono>
#include "matrix.h"

namespace po = boost::program_options;
using namespace std;

Tmat RandomMatrix(int m, int n, std::uniform_real_distribution<double> urd, std::default_random_engine re);

enum Method {
  CLASSIC, CLASSIC_T, RECURSIVE, RECURSIVE_T, SUBCUBIC
};
static map<string, Method> methodMap{
    {"classic",              CLASSIC},
    {"classic_transposed",   CLASSIC_T},
    {"recursive",            RECURSIVE},
    {"recursive_transposed", RECURSIVE_T},
    {"subcubic",             SUBCUBIC},
};

int main(int ac, const char **av) {
  int a, b, c;
  int repeat;
  int seed = 1; //totaly random
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
        )
        ("a,a", po::value<int>()->default_value(512), "A in (A,B)x(B,C) mutiplication  \n ")
        ("b,b", po::value<int>()->default_value(512), "B in (A,B)x(B,C) mutiplication  \n ")
        ("c,c", po::value<int>()->default_value(512), "C in (A,B)x(B,C) mutiplication  \n ")
        ("repeat,r", po::value<int>()->default_value(1), "Number of tests to run");

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
        cout << "testing with method " << m << '\n';
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
      cout << "a = " << a << '\n';

    } else {
      cout << "a was not set!\n";
      return 1;
    }
    if (vm.count("b")) {
      b = vm["b"].as<int>();
      cout << "b = " << b << '\n';

    } else {
      cout << "b was not set!\n";
      return 1;
    }
    if (vm.count("c")) {
      c = vm["c"].as<int>();
      cout << "c = " << c << '\n';

    } else {
      cout << "c was not set!\n";
      return 1;
    }

    repeat = vm["repeat"].as<int>();
    cout << "Number of test: " << repeat << endl;

  } catch (std::exception &e) {
    cerr << e.what();
  }

  //Execute the tests


  std::uniform_real_distribution<double> urd(0, 1);
  std::default_random_engine re(seed);
  Tmat m1 = RandomMatrix(a,b,urd,re);
  Tmat m2 = RandomMatrix(b,c,urd,re);

  std::function<Tmat(Tmat &, Tmat &)> f;

  switch (method) {
    case CLASSIC:
      f = klasicno_mnozenje;
      break;
    case CLASSIC_T:
      f = transponirano_mnozenje;
      break;
    case RECURSIVE:
      f = rekurzivno_mnozenje;
      break;
    case RECURSIVE_T:
      f = rekurzivno_transponirano_mnozenje;
      break;
    case SUBCUBIC:
      f = podkubicen;
      break;
  }

  std::chrono::time_point<std::chrono::steady_clock> _TEST_start, _TEST_end;
  _TEST_start = std::chrono::steady_clock::now();

  for (int i = 0; i < repeat; ++i) {
    Tmat m3 = f(m1,m2);
  }

  _TEST_end = std::chrono::steady_clock::now();
  auto _TEST_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(_TEST_end - _TEST_start);
  std::cout << _TEST_elapsed.count() << " ms\n";

}




Tmat RandomMatrix(int m, int n, std::uniform_real_distribution<double> urd, std::default_random_engine re) {
  Tmat mat = newMat(m, n);
  for (int i = 0; i < m; i += 1) {
    for (int j = 0; j < n; j += 1) {
      mat[i][j] = urd(re);
    }
  }
  return mat;
}

