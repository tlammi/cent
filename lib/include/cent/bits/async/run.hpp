#pragma once
#include <cent/bits/async/executor.hpp>
#include <cent/bits/async/util.hpp>
#include <type_traits>

namespace cent::async {

template <class T>
T run(Task<T> t) {
    if constexpr (std::same_as<T, void>) {
        auto e = Executor{std::move(t)};
        while (!e.done()) e.resume();
    } else {
        T res{};
        auto e = Executor{to_void_task(&res, std::move(t))};
        while (!e.done()) e.resume();
        return res;
    }
}

template <std::same_as<Task<void>>... Ts>
void run_all(Ts&&... ts) {
    auto e = Executor(std::forward<Ts>(ts)...);
    while (!e.done()) e.resume();
}

}  // namespace cent::async
