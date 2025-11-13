#include <cent/dist/http/smart_session.hpp>
#include <cent/dist/registry_client.hpp>
#include <cent/exception.hpp>
#include <print>

void run(int argc, char** argv) {
    if (argc != 2)
        cent::raise(cent::ErrorCode::MissingArgument, "usage: {} URL", argv[0]);

    auto raw_sess = cent::dist::http::Session();
    auto smart_sess = cent::dist::http::SmartSession(raw_sess);
    auto client = cent::dist::RegistryClient(smart_sess);
    auto res = client.manifest(argv[1]);
    std::println("{}", rfl::json::write(res));
}

int main(int argc, char** argv) {
    try {
        run(argc, argv);
    } catch (const std::exception& ex) {
        std::println(stderr, "{}", ex.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
