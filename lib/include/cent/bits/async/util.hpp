#pragma once

#include <cent/bits/async/task.hpp>
#include <type_traits>
#include <utility>

namespace cent::async {

template <class T>
Task<void> to_void_task(T* out, std::remove_cvref_t<Task<T>>&& t) {
    *out = co_await std::move(t);
}

template <class T>
Task<void> to_void_task(T* out, std::remove_cvref_t<Task<T>>& t) {
    *out = co_await t;
}

}  // namespace cent::async
