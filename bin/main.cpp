#include <iostream>

#include "cent.hpp"
#include "interface.hpp"
int main() {
    Interface i{};
    cent::Cent c{&i};
    std::cerr << c.pull("docker.io/library/ubuntu:20.04") << '\n';
}
