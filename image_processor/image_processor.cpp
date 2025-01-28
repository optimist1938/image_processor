#include <string>
#include "ImageInfo/Image.h"
#include "ImageInfo/Image.cpp"
#include "ImageInfo/RGB.cpp"
#include "ReadBmp/ReadBmp.cpp"
#include "ReadBmp/ReadBmp.h"
#include "ReadBmp/parser.cpp"
#include <stdexcept>
#include "fstream"
#include <iostream>
#include "Filters/filter.cpp"
int main(int argc, char** argv) {
    try {
        std::vector<std::pair<Filter*, std::vector<std::string> > > filters_to_do;
        Image image;
        filters_to_do = Parser(argc, argv);  // NOLINT
        std::string path_in = argv[1];
        std::string path_out = argv[2];
        image.Read(path_in);
        for (auto [filter_name, parametrs] : filters_to_do) {  // NOLINT
            filter_name->Do(image, parametrs);
            delete filter_name;
        }
        SaveFile(path_out, image);
    } catch (std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}