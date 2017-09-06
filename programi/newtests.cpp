#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
using namespace std;

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
  int size;
  int repeat;
  Method method;
  try {
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help,h", "Display this message")
        ("method,m", po::value<string>()->default_value("subcubic"), "Method to benchmark \n"
            "Available methods:\n"
            "  classic\n"
            "  classic_transposed\n"
            "  recursive\n"
            "  recursive_transposed\n"
            "  subcubic \n"
        )
        ("size,s", po::value<int>()->default_value(512), "Size of matrix (NxN) \n ")
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

    if (vm.count("size")) {
      size = vm["size"].as<int>();
      cout << "size = " << size << '\n';

    } else {
      cout << "size was not set!\n";
      return 1;
    }

    repeat = vm["repeat"].as<int>();
    cout << "Number of test: " << repeat << endl;

  } catch (std::exception &e) {
    cerr << e.what();
  }
}