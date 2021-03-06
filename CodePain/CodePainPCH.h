#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#pragma warning(pop)

#include <stdio.h>
#include <iostream> // std::cout
#include <sstream> // stringstream
#include <memory> // smart pointers
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define MULTI_THREADING

#define FLT_EPSILON      1.192092896e-07F
#define SAFE_DELETE( ptr ) if (ptr != nullptr) { delete ptr; ptr = nullptr; }

template <class T>
bool IS_VALID(T* ptr)
{
    return ptr != nullptr;
}