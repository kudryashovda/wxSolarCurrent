#pragma once

#include "interpolator.h"

class Integrator {
public:
    Integrator(double first, double last, const std::string& str_data, char delimLines, char delimData, std::string filename);

    double getSolarCurrent();
    size_t getAmstDataSize();
private:
    std::vector<Coord> amstData_;
    std::vector<Coord> eqeData_;
    std::vector<Coord> wl_NqData_;
    double solarCurrent_ = 0;
};
