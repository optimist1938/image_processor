#include <string>
#include "../Filters/filter.h"
#include <iostream>
std::vector<std::pair<Filter*, std::vector<std::string> > > Parser(int argc, char** argv) {
    std::vector<std::pair<Filter*, std::vector<std::string> > > filters_to_do;
    for (int i = 3; i < argc; ++i) {
        std::string argument = argv[i];
        if (argument == "-crop") {
            FilterCrop* fitler_crop = new FilterCrop;
            std::vector<std::string> parameters;
            parameters.push_back(argv[i + 1]);
            parameters.push_back(argv[i + 2]);
            filters_to_do.push_back(std::make_pair(fitler_crop, parameters));
            ++ ++i;
        } else if (argument == "-gs") {
            std::vector<std::string> parameters;
            FilterGrayscale* filter_grayscale = new FilterGrayscale;
            filters_to_do.push_back(std::make_pair(filter_grayscale, parameters));
        } else if (argument == "-neg") {
            std::vector<std::string> parameters;
            FilterNegative* filter_negative = new FilterNegative;
            filters_to_do.push_back(std::make_pair(filter_negative, parameters));
        } else if (argument == "-sharp") {
            std::vector<std::string> parameters;
            FilterSharpening* filter_sharp = new FilterSharpening;
            filters_to_do.push_back(std::make_pair(filter_sharp, parameters));
        } else if (argument == "-edge") {
            std::vector<std::string> parameters;
            parameters.push_back(argv[i + 1]);
            ++i;
            FilterEdgeDetection* filter_edge = new FilterEdgeDetection;
            filters_to_do.push_back(std::make_pair(filter_edge, parameters));
        } else if (argument == "-blur") {
            std::vector<std::string> parameters;
            parameters.push_back(argv[i + 1]);
            FilterBlur* filter_blur = new FilterBlur;
            ++i;
            filters_to_do.push_back(std::make_pair(filter_blur, parameters));
        }  else if (argument == "-water") {
            FilterWaterDroplets* filter_water = new FilterWaterDroplets;
            std::vector<std::string> parameters;
            parameters.push_back(argv[i + 1]);
            filters_to_do.push_back(std::make_pair(filter_water, parameters));
            ++i;
        } else {
            throw std::runtime_error("Parametrs can not be read");
        } 
    }
    return filters_to_do;
}