#include <iostream>
#include <boost/program_options.hpp>


namespace po = boost::program_options;
using namespace std;

int main(int ac, const char **av) {

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "Display this message")
            ("compression", po::value<int>(), "set compression level")
            ("method", po::value<string>(), "Method to benchmark \n"
                    "Available methods:\n"
                    "  classic\n"
                    "  classic_transposed\n"
                    "  recursive\n"
                    "  recursive_transposed\n"
                    "  subcubic --?\n"
            )
            ("size", po::value<int>(), "size of matrix (NxN) \n ");

    po::variables_map vm;
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    if (vm.count("compression")) {
        cout << "Compression level was set to "
             << vm["compression"].as<int>() << ".\n";
    } else {
        cout << "Compression level was not set.\n";
    }
    if (vm.count("method")) {
        cout << "testing with method " + vm["method"].as<string>();
    } else {
        cout << "method was not set";
    }
}