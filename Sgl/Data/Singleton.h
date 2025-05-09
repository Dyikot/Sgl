#pragma once

#define Sgl_Singleton(T, varName, value) []() -> T& { static T varName = T(value); return varName; }
#define Sgl_ConstSingleton(T, varName, value) []() -> const T& { static T varName = T(value); return varName; }