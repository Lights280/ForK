#pragma once

#include "glad/glad.h"
#include "alc.h"
#include "al.h"
#include "glfw3.h"

#include <bits/stdc++.h>

#include <chrono>
#include <deque>
#include <vector>

constexpr float SPEED = 0.003f;
constexpr GLuint VSIZE = sizeof(float) * 3;
constexpr int KEY[4] = {GLFW_KEY_D, GLFW_KEY_F, GLFW_KEY_J, GLFW_KEY_K};

constexpr float game_vertices[] = {
    // Line 1
    -0.2f,
    -1.0f,
    0.0f,
    -0.2f,
    1.0f,
    0.0f,
    // Line 2
    0.1f,
    -1.0f,
    0.0f,
    0.1f,
    1.0f,
    0.0f,
    // Line 3
    0.4f,
    -1.0f,
    0.0f,
    0.4f,
    1.0f,
    0.0f,
    // Li ne 4
    0.7f,
    -1.0f,
    0.0f,
    0.7f,
    1.0f,
    0.0f,
    // Line 5
    1.0f,
    -1.0f,
    0.0f,
    1.0f,
    1.0f,
    0.0f,
    // Line 6
    -0.2f,
    -0.8f,
    0.0f,
    1.0f,
    -0.8f,
    0.0f,
    // Square 1
    -0.2f,
    -0.8f,
    0.0f,
    -0.2f,
    -1.0f,
    0.0f,
    0.1f,
    -0.8f,
    0.0f,
    0.1f,
    -1.0f,
    0.0f,
    // Square 2
    0.1f,
    -0.8f,
    0.0f,
    0.1f,
    -1.0f,
    0.0f,
    0.4f,
    -0.8f,
    0.0f,
    0.4f,
    -1.0f,
    0.0f,
    // Sqrare 3
    0.4f,
    -0.8f,
    0.0f,
    0.4f,
    -1.0f,
    0.0f,
    0.7f,
    -0.8f,
    0.0f,
    0.7f,
    -1.0f,
    0.0f,
    // Square 4
    0.7f,
    -0.8f,
    0.0f,
    0.7f,
    -1.0f,
    0.0f,
    1.0f,
    -0.8f,
    0.0f,
    1.0f,
    -1.0f,
    0.0f,
};

const std::function<void(const char *)> error = [](const char *message)
{ std::cerr << "[Error]:" << message << std::endl; },
                                        info = [](const char *message)
{ std::cerr << "[Info]:" << message << std::endl; };