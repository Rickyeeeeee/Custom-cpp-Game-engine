#pragma once

#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using Scope = std::unique_ptr<T>;

#define BIND_EVENT(fn) std::bind(&fn, this, std::placeholders::_1)