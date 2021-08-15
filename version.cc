#include <boost/version.hpp>
#include <cstdio>

int main(int argc, const char** argv) {
    std::printf("Boost version: %d.%d.%d\n",
        BOOST_VERSION / 100000,
        (BOOST_VERSION / 100) % 1000,
        BOOST_VERSION % 100);
    
    return 0;
}
