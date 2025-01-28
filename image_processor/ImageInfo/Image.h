#pragma once
#include <vector>
#include "RGB.h"
#include <string>

class Image {
public:
    int width_;
    int height_;
    int x_pixels_per_m_;
    int y_pixels_per_m_;
    std::vector<std::vector<RGB> > pixels_;

public:
    Image();
    Image(int width, int height);
    ~Image() = default;  // NOLINT
    RGB GetRgb(int x, int y) const;
    void ChangePixel(RGB rgb, int x, int y);
    int Width() const;
    int Height() const;
    int GetXPixels() const;
    int GetYPixels() const;
    friend void SaveFile(std::string &path, Image &image);
    void SetRGB(RGB colour, const int x, const int y);
    void Read(std::string &path);

    RGB BoarderDetection(const int x, const int y) const;
    bool cropped = false;  // NOLINT
};