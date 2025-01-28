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