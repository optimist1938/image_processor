#pragma once

struct RGB {
    float r_ = 0;  // NOLINT
    float g_ = 0;  // NOLINT
    float b_ = 0;  // NOLINT

    ~RGB() = default;  // NOLINT
    RGB();
    RGB(float r, float g, float b);
    RGB &operator=(const RGB &rgb);
};