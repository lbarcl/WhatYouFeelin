#pragma once
#include <vector>
#include <cmath>

const int NUM_SEGMENTS = 100;
const float PI = 3.1415926f;

std::vector<float> generateCircleVertices(float cx, float cy, float r, int num_segments = NUM_SEGMENTS)
{
    std::vector<float> vertices;

    // Center of circle
    vertices.push_back(cx);
    vertices.push_back(cy);

    // Edge points
    for (int i = 0; i <= num_segments; ++i)
    {
        float angle = 2.0f * PI * float(i) / float(num_segments);
        float x = r * cosf(angle);
        float y = r * sinf(angle);
        vertices.push_back(cx + x);
        vertices.push_back(cy + y);
    }

    return vertices;
}
