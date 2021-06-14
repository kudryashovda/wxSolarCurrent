#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

struct Coord {
    double x = 0.0;
    double y = 0.0;
};

std::vector<std::string> SplitIntoTerms(const std::string& text, char delim);
std::vector<Coord> SplitIntoData(const std::string& text, char delimLines, char delimData);
std::string ConvertToString(std::string filename);

class Interpolator {
public:
    explicit Interpolator(double first, double last, double step, const std::vector<Coord>& input_data);
    explicit Interpolator(double first, double last, double step, const std::string& str_data, char delimLines, char delimData);
    explicit Interpolator(double first, double last, double step, std::string filename);

    std::vector<Coord> getInterpolatedData();
private:
    std::vector<Coord> m_output_data;
};
