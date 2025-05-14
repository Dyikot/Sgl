#pragma once

#define Sgl_Singleton(T, value) []() -> T& { static T var = T(value); return var; }
#define Sgl_ConstSingleton(T, value) []() -> const T& { static T var = T(value); return var; }