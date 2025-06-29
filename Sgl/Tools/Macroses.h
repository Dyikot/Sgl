#pragma once

#define SINGLETON(T, value) []() -> T& { static T var = T(value); return var; }
#define CONST_SINGLETON(T, value) []() -> const T& { static T var = T(value); return var; }