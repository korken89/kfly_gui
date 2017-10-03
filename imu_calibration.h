#ifndef IMU_CALIBRATION_H
#define IMU_CALIBRATION_H

#include <vector>
#include <eigen3/Eigen/Dense>

namespace calibration
{

enum class state
{
    GETTING_SAMPLES,
    NEXT_AXIS,
    DONE
};

class gain_bias_3axis
{
private:
    std::vector<double> samples_x_;
    std::vector<double> samples_y_;
    std::vector<double> samples_z_;

    Eigen::Matrix<double, 6, 3> axis_means_;
    Eigen::Vector3d axis_variance_;
    int current_axis_;

    Eigen::Vector3d gain_;
    Eigen::Vector3d bias_;

    state current_state_;

    void apply_median_filter();
    double mean(const std::vector<double> &vec);
    double variance(const std::vector<double> &vec);
    double sample_mean();
    double sample_variance();
    void estimate();

    unsigned num_samples_;

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    gain_bias_3axis(unsigned num_samples);

    state add_sample(double x, double y, double z);
    state axis_finished();
    int get_axis_percent_done();

    Eigen::Vector3d get_gain_result();
    Eigen::Vector3d get_bias_result();
    Eigen::Vector3d get_variance();

    void reset();
    bool valid();
};

} // end imu_calibration

#endif // IMU_CALIBRATION_H
