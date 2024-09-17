#include <cent/thirdparty/clipp.h>

#include <cent/net.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    std::string_view request{};
    std::vector<std::string_view> headers{};
    std::string_view url{};
    clipp::group cli = (clipp::option("--help").call([&] {
        std::cout << clipp::make_man_page(cli, argv[0]);
        ::exit(EXIT_SUCCESS);
    }) % "show this help",
                        clipp::option("-h", " --header").set(headers) %
                            "Request headers. Can be given multiple times",
                        clipp::option("-r", "--request").set(request) %
                            "Set request type. Default: 'GET'",
                        clipp::value("url", url) % "Target URL");
    if (!clipp::parse(argc, argv, cli)) return EXIT_FAILURE;

    auto sess = cent::net::Session();
}
