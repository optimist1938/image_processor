#include "filter.h"
#include <algorithm>
#include <string>
#include <cmath>
#include <iostream>

RGB Graing(const RGB& colour) {
    float gray = colour.r_ * 0.299 + colour.g_ * 0.587 + colour.b_ * 0.114;  // NOLINT
    RGB colour2;
    colour2.r_ = gray;
    colour2.g_ = gray;
    colour2.b_ = gray;
    return colour2;
}

void Grayscale(Image& image) {
    int width = image.Width();
    int height = image.Height();
    Image tmp(width, height);
    for (int x = 0; x != height; ++x) {
        for (int y = 0; y != width; ++y) {
            tmp.ChangePixel(Graing(image.GetRgb(y, x)), y, x);
        }
    }
    image = tmp;
}

void FilterGrayscale::Do(Image& image, std::vector<std::string>) {
    Grayscale(image);
}

void Negative(Image& image) {
    int width = image.Width();
    int height = image.Height();
    for (int x = 0; x != height; ++x) {
        for (int y = 0; y != width; ++y) {
            RGB pixel = image.GetRgb(y, x);
            float r = 1.0f - pixel.r_;
            float g = 1.0f - pixel.g_;
            float b = 1.0f - pixel.b_;
            r = std::min(std::max(r, 0.0f), 1.0f);
            g = std::min(std::max(g, 0.0f), 1.0f);
            b = std::min(std::max(b, 0.0f), 1.0f);
            image.ChangePixel(RGB(r, g, b), y, x);
        }
    }
}
void FilterNegative::Do(Image& image, std::vector<std::string>) {
    Negative(image);
}

void Crop(Image& image, int ceiling, int down) {
    int ceiling2 = std::min(ceiling, image.Width());
    int down2 = std::min(down, image.Height());
    Image tmp(ceiling2, down2);
    for (int x = 0; x < down2; ++x) {
        for (int y = 0; y < ceiling2; ++y) {
            RGB colour = image.GetRgb(y, image.Height() - down2 + x);
            tmp.ChangePixel(colour, y, x);
        }
    }
    image = tmp;
}
void FilterCrop::Do(Image& image, std::vector<std::string> parameters) {
    Crop(image, std::stoi(parameters[0]), std::stoi(parameters[1]));
}

void EdgeDetection(Image& image, float threshold) {
    RGB pixel;
    Grayscale(image);
    Image temp = Image(image);
    for (int y = 0; y < image.Height(); ++y) {
        for (int x = 0; x < image.Width(); ++x) {
            RGB rgb1 = temp.BoarderDetection(y, x);
            RGB rgb2 = temp.BoarderDetection(y, x - 1);
            RGB rgb3 = temp.BoarderDetection(y, x + 1);
            RGB rgb4 = temp.BoarderDetection(y - 1, x);
            RGB rgb5 = temp.BoarderDetection(y + 1, x);
            pixel.r_ = static_cast<float>(4) * rgb1.r_ - rgb2.r_ - rgb3.r_ - rgb4.r_ - rgb5.r_;
            pixel.g_ = static_cast<float>(4) * rgb1.g_ - rgb2.g_ - rgb3.g_ - rgb4.g_ - rgb5.g_;
            pixel.b_ = static_cast<float>(4) * rgb1.b_ - rgb2.b_ - rgb3.b_ - rgb4.b_ - rgb5.b_;
            if (pixel.r_ > threshold || pixel.g_ > threshold || pixel.b_ > threshold) {
                pixel.r_ = 1.0f;
                pixel.g_ = 1.0f;
                pixel.b_ = 1.0f;
            } else {
                pixel.r_ = 0.0f;
                pixel.g_ = 0.0f;
                pixel.b_ = 0.0f;
            }
            image.ChangePixel(pixel, x, y);
        }
    }
}

void FilterEdgeDetection::Do(Image& image, std::vector<std::string> parameters) {
    EdgeDetection(image, std::stof(parameters[0]));
}

std::vector<float> CalculateWeights(float sigma) {
    std::vector<float> res;
    int radius = std::ceil(4 * sigma);
    float value = 0;
    for (int i = -radius; i <= radius; ++i) {
        value = exp(-powf(i, 2.0f) / (2 * powf(sigma, 2.0f))) / sqrt(2 * atan(1) * 4 * powf(sigma, 2.0f));  // NOLINT
        res.push_back(value);
    }
    return res;
}

void ApplyHorizontalBlur(Image& image, std::vector<float>& weights, int radius) {
    Image temp = image;
    for (int y = 0; y < image.Height(); ++y) {
        for (int x = 0; x < image.Width(); ++x) {
            float red = 0.0f;
            float green = 0.0f;
            float blue = 0.0f;
            for (int i = 0; i < 2 * radius + 1; ++i) {
                RGB temp_pixel = temp.BoarderDetection(y, x + i - radius);
                red += weights[i] * temp_pixel.r_;
                green += weights[i] * temp_pixel.g_;
                blue += weights[i] * temp_pixel.b_;
            }
            image.ChangePixel(RGB(red, green, blue), x, y);
        }
    }
}

void ApplyVerticalBlur(Image& image, std::vector<float>& weights, int radius) {
    Image temp = image;
    for (int y = 0; y < image.Height(); ++y) {
        for (int x = 0; x < image.Width(); ++x) {
            float red = 0.0f;
            float green = 0.0f;
            float blue = 0.0f;
            for (int i = 0; i < 2 * radius + 1; ++i) {
                RGB temp_pixel = temp.BoarderDetection(y + i - radius, x);
                red += weights[i] * temp_pixel.r_;
                green += weights[i] * temp_pixel.g_;
                blue += weights[i] * temp_pixel.b_;
            }
            image.ChangePixel(RGB(red, green, blue), x, y);
        }
    }
}

void FilterBlur::Do(Image& image, std::vector<std::string> argument) {
    float sigma = std::stof(argument[0]);
    int radius = std::ceil(4 * sigma);
    std::vector<float> weights = CalculateWeights(sigma);
    ApplyHorizontalBlur(image, weights, radius);
    ApplyVerticalBlur(image, weights, radius);
}

void Sharpening(Image& image) {
    Image tmp = image;
    for (int x = 0; x != image.Height(); ++x) {
        for (int y = 0; y != image.Width(); ++y) {
            float r = image.BoarderDetection(x, y).r_;
            float g = image.BoarderDetection(x, y).g_;
            float b = image.BoarderDetection(x, y).b_;
            r *= 5;  // NOLINT
            r -= image.BoarderDetection(x - 1, y).r_;
            r -= image.BoarderDetection(x + 1, y).r_;
            r -= image.BoarderDetection(x, y - 1).r_;
            r -= image.BoarderDetection(x, y + 1).r_;
            g *= 5;  // NOLINT
            g -= image.BoarderDetection(x - 1, y).g_;
            g -= image.BoarderDetection(x + 1, y).g_;
            g -= image.BoarderDetection(x, y - 1).g_;
            g -= image.BoarderDetection(x, y + 1).g_;
            b *= 5;  // NOLINT
            b -= image.BoarderDetection(x - 1, y).b_;
            b -= image.BoarderDetection(x + 1, y).b_;
            b -= image.BoarderDetection(x, y - 1).b_;
            b -= image.BoarderDetection(x, y + 1).b_;
            r = std::min(std::max(r, 0.0f), 1.0f);
            g = std::min(std::max(g, 0.0f), 1.0f);
            b = std::min(std::max(b, 0.0f), 1.0f);
            tmp.ChangePixel(RGB(r, g, b), y, x);
        }
    }
    image = tmp;
}

void FilterSharpening::Do(Image& image, std::vector<std::string>) {
    return Sharpening(image);
}


int ExtractSquareSize(const std::vector<std::string>& parameters) {
    int square_size; //NOLINT
    try {
        square_size = std::stoi(parameters[0]);
    } catch (const std::invalid_argument& e) {
        std::cout << "Invalid command line argument" << std::endl;
        return -1;
    }
    return square_size;
}

bool ValidateSquareSize(int square_size, const Image& image) {
    int max_square_size = std::min(image.Width(), image.Height()) / 4;
    if (square_size > max_square_size) {
        std::cout << "Invalid command line argument" << std::endl;
        return false;
    }
    
    return true;
}

void ApplyMosaicFilter(const Image& image, Image& image2, int square_size) {
    for (int x = 0; x < image.Height(); x++) {
        for (int y = 0; y < image.Width(); y++) {
            if (x < square_size || y < square_size || image.Height() - x < square_size || image.Width() - y < square_size) {
                image2.ChangePixel(RGB(0, 0, 0), y, x);
            } else if (x % square_size == 0 && y % square_size == 0) {
                unsigned char avg_r = 0, avg_g = 0, avg_b = 0; //NOLINT
                int counter = 0;
                for (int i = x; i < std::min(x + square_size, image.Height()); i++) {
                    for (int j = y; j < std::min(y + square_size, image.Width()); j++) {
                        RGB rgb = image.GetRgb(j, i);
                        avg_r += rgb.r_; //NOLINT
                        avg_g += rgb.g_; //NOLINT
                        avg_b += rgb.b_; //NOLINT
                        counter++;
                    }
                }
                avg_r /= counter;
                avg_g /= counter;
                avg_b /= counter;
                avg_r = std::max(0, std::min(255, static_cast<int>(avg_r))); //NOLINT
                avg_g = std::max(0, std::min(255, static_cast<int>(avg_g))); //NOLINT
                avg_b = std::max(0, std::min(255, static_cast<int>(avg_b))); //NOLINT
                for (int i = x; i < std::min(x + square_size, image.Height()); i++) {
                    for (int j = y; j < std::min(y + square_size, image.Width()); j++) {
                        image2.ChangePixel(RGB(avg_r, avg_g, avg_b), j, i);
                    }
                }
            }
        }
    }
}

void Mosaic(const Image& image, std::vector<std::string> parameters) {
    Image image2 = Image(image.Width(), image.Height());
    
    int square_size = ExtractSquareSize(parameters);
    if (square_size == -1 || !ValidateSquareSize(square_size, image)) {
        return;
    }
    ApplyMosaicFilter(image, image2, square_size);
}

void FilterWaterDroplets::Do(Image& image, std::vector<std::string> params) { //NOLINT
        int drop_radius = std::stoi(params[0]);
        return Mosaic(image, params);
    }
