#pragma once

#include <memory>
namespace cent {

template <class T>
using UniqPtr = std::unique_ptr<T>;

template <class T>
using ShrdPtr = std::shared_ptr<T>;

}  // namespace cent
