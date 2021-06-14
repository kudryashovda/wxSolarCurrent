#include "integrator.h"

using namespace std;

Integrator::Integrator(double first, double last, const std::string& str_data, char delimLines, char delimData, std::string filename) {
    // amst tabulated data range
    constexpr double first_limit = 280;
    constexpr double last_limit = 4000;

    if (last < first || first < first_limit || last > last_limit) {
        return;
    }

    constexpr int step = 1;
    Interpolator amst_interpolated(first, last, step, filename);
    amstData_ = amst_interpolated.getInterpolatedData(); // mb empty

    Interpolator eqe_interpolated(first, last, step, str_data, delimLines, delimData);
    eqeData_ = eqe_interpolated.getInterpolatedData(); // mb empty

    for(size_t i = 0; i < eqeData_.size(); ++i) {
        wl_NqData_.push_back({eqeData_[i].x, eqeData_[i].y * amstData_[i].y});
    }

    double summ = 0; // in A/m^2

    for(size_t i = 0; (i + 1) < wl_NqData_.size(); ++i) {
        double y_mid = (wl_NqData_[i+1].y + wl_NqData_[i].y) / 2;
        double dx = wl_NqData_[i+1].x - wl_NqData_[i].x;

        summ += y_mid * dx;
    }

    solarCurrent_ = summ / 10; // in mA/cm^2
}

double Integrator::getSolarCurrent() {
    return solarCurrent_;
}

size_t Integrator::getAmstDataSize() {
    return amstData_.size();
}
