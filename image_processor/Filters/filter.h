#pragma once

#include "../ImageInfo/Image.h"

class Filter {
public:
    virtual void Do(Image& image, std::vector<std::string>) = 0;
    virtual ~Filter(){};
};

class FilterBlur : public Filter {
public:
    void Do(Image& image, std::vector<std::string>) override;  // NOLINT
    ~FilterBlur() override = default;                          // NOLINT
};

void Blur(Image& image, const float& sigma2);

class FilterGrayscale : public Filter {
public:
    void Do(Image& image, std::vector<std::string>) override;  // NOLINT
    ~FilterGrayscale() override = default;                     // NOLINT
};

void Grayscale(Image& image);

class FilterNegative : public Filter {
public:
    void Do(Image& image, std::vector<std::string>) override;  // NOLINT
    ~FilterNegative() override = default;                      // NOLINT
};

void Negative(const Image& image);

class FilterCrop : public Filter {
public:
    void Do(Image& image, std::vector<std::string>) override;  // NOLINT
    ~FilterCrop() override = default;                          // NOLINT
};

void Crop(Image& image, int down, int right);

class FilterEdgeDetection : public Filter {
public:
    void Do(Image& image, std::vector<std::string>) override;  // NOLINT
    ~FilterEdgeDetection() override = default;                 // NOLINT
};

void EdgeDetection(Image& image, unsigned char threshold);

class FilterSharpening : public Filter {
public:
    void Do(Image& image, std::vector<std::string>) override;  // NOLINT
    ~FilterSharpening() override = default;                    // NOLINT
};

void Sharpening(Image& image);

class FilterWaterDroplets : public Filter {
public:
    void Do(Image& image, std::vector<std::string>) override;  // NOLINT
    ~FilterWaterDroplets() override = default;                    // NOLINT
};

void Mosaic(const Image& image);