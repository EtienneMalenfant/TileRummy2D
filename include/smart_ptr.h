#pragma once
#include <memory>

template <typename T>
using ptr = std::shared_ptr<T>;

template <typename T>
using uptr = std::unique_ptr<T>;
