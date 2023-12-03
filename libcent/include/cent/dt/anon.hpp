#pragma once

#include <memory>
#include <utility>

namespace cent {

namespace anon_detail {

template <class T>
void dtor(void* ptr) noexcept {
    delete static_cast<T*>(ptr);
}

}  // namespace anon_detail

class Anon {
 public:
    constexpr Anon() = default;
    template <class T, class... Ts>
    explicit Anon(std::in_place_type_t<T> /*in_place*/, Ts&&... ts)
        : m_ptr(new T(std::forward<Ts>(ts)...), anon_detail::dtor<T>) {}

    Anon(const Anon&) = delete;
    Anon& operator=(const Anon&) = delete;

    Anon(Anon&&) noexcept = default;
    Anon& operator=(Anon&&) noexcept = default;

    ~Anon() = default;

    void* data() const noexcept { return m_ptr.get(); }

    void reset() { m_ptr.reset(); }

 private:
    std::unique_ptr<void, void (*)(void*)> m_ptr{nullptr, nullptr};
};
}  // namespace cent
