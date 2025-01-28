#include "Image.h"
#include <algorithm>
#include <string>

Image::Image() : width_(0), height_(0) {
}

Image::Image(int width, int height)
    : width_(width), height_(height), pixels_(height_, std::vector<RGB>(width_, RGB())) {
}

int Image::Width() const {
    return width_;
}

int Image::Height() const {
    return height_;
}

RGB Image::GetRgb(int x, int y) const {
    return pixels_[y][x];
}

int Image::GetXPixels() const {
    return x_pixels_per_m_;
}
int Image::GetYPixels() const {
    return y_pixels_per_m_;
}
void Image::ChangePixel(RGB rgb, int x, int y) {
    pixels_[y][x] = rgb;
}

RGB Image::BoarderDetection(const int x, const int y) const {
    auto x2 = std::min(std::max(x, 0), height_ - 1);  // NOLINT
    auto y2 = std::min(std::max(y, 0), width_ - 1);   // NOLINT
    return this->GetRgb(y2, x2);
}