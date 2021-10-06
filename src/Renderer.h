#pragma once

#include "pch.h"

#include <glad/glad.h>

#pragma region Error Handling

#define ASSERT(x) if (!(x)) __debugbreak();
#define GL_CALL(x) GLClearError();\
                   x;\
                   ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

# pragma endregion

class Renderer
{
public:
    void Clear() const;
    void Draw();
};