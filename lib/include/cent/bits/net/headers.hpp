#pragma once
#include <cent/str.hpp>
#include <memory>

namespace cent::net {

/**
 * \brief HTTP headers
 * */
class Headers {
 public:
    constexpr Headers() noexcept = default;

    Headers(std::initializer_list<std::pair<std::string_view, std::string_view>>
                args);

    Headers(std::initializer_list<CStr> args);

    ~Headers();

    size_t size() const noexcept;

    bool empty() const noexcept { return size() == 0; }

 private:
    class Deleter {
     public:
        void operator()(void* ptr);
    };
    std::unique_ptr<void, Deleter> m_ptr{};
};

}  // namespace cent::net
