#pragma once

#include <memory>
#include <utility>
namespace cent {

template <class T>
using Ptr = std::unique_ptr<T>;

template <class T, class... Ts>
Ptr<T> make_ptr(Ts&&... ts) {
    return std::make_unique<T>(std::forward<Ts>(ts)...);
}

template <class T>
using ShrdPtr = std::shared_ptr<T>;

}  // namespace cent
