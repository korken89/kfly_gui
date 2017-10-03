#include "imu_calibration.h"

#include <algorithm>
#include <numeric>

namespace calibration
{

void gain_bias_3axis::apply_median_filter()
{
    std::sort(samples_x_.begin(), samples_x_.end());
    std::sort(samples_y_.begin(), samples_y_.end());
    std::sort(samples_z_.begin(), samples_z_.end());

    // Simple outlier rejector
    auto tail_size = samples_x_.size() / 10; // Remove top / bottom 10%

    samples_x_ = std::vector<double>(samples_x_.begin() + tail_size, samples_x_.end() - tail_size);
    samples_y_ = std::vector<double>(samples_y_.begin() + tail_size, samples_y_.end() - tail_size);
    samples_z_ = std::vector<double>(samples_z_.begin() + tail_size, samples_z_.end() - tail_size);
}

double gain_bias_3axis::mean(const std::vector<double> &vec)
{
    double sum = std::accumulate(vec.cbegin(), vec.cend(), 0.0);
    double mean = sum / vec.size();

    return mean;
}

double gain_bias_3axis::variance(const std::vector<double> &vec)
{
    double sum = std::accumulate(vec.cbegin(), vec.cend(), 0.0);
    double sq_sum = std::inner_product(vec.cbegin(), vec.cend(), vec.cbegin(), 0.0);

    double mean = sum / vec.size();
    double variance = sq_sum / vec.size() - mean * mean;

    return variance;
}

void gain_bias_3axis::estimate()
{
/* Matlab code:

    % Fit an ellipsoid in the form Ax^2 + By^2 + Cz^2 + 2Gx + 2Hy + 2Iz = 1
    D = [ x .* x + y .* y - 2 * z .* z, ...
          x .* x + z .* z - 2 * y .* y, ...
          2 * x, ...
          2 * y, ...
          2 * z, ...
          1 ];

    d = x .^2 + y .^2 + z .^2; % the RHS of the llsq problem (y's)

    %[Q,R] = qr(D); % Use QR for greater numerical stability
    %u = R\(Q'*d);
    u = ( D' * D ) \ ( D' * d );  % solution to the normal equations

    v1 = [    u(1) + u(2) - 1;
          u(1) - 2 * u(2) - 1;
          u(2) - 2 * u(1) - 1];

    v2 = u(3:5);

    c = u (6);

    bias = - v2 ./ v1;

    gain = sqrt(-v1 / (bias'*v2 + c));
*/
    using Vector6d = Eigen::Matrix<double, 6, 1>;

    Eigen::Matrix<double, 6, 6> D;

    Vector6d x = axis_means_.col(0);
    Vector6d y = axis_means_.col(1);
    Vector6d z = axis_means_.col(2);

    // Least squares problem solving
    D.col(0) = x.cwiseProduct(x) + y.cwiseProduct(y) - 2.0 * z.cwiseProduct(z);
    D.col(1) = x.cwiseProduct(x) + z.cwiseProduct(z) - 2.0 * y.cwiseProduct(y);
    D.col(2) = 2.0 * x;
    D.col(3) = 2.0 * y;
    D.col(4) = 2.0 * z;
    D.col(5).setOnes();

    Vector6d d = x.cwiseProduct(x) + y.cwiseProduct(y) + z.cwiseProduct(z);

    Vector6d u = D.colPivHouseholderQr().solve(d);

    // Generate output
    Eigen::Vector3d v1, v2;

    v1(0) = u(0) + 		 u(1) - 1.0;
    v1(1) = u(0) - 2.0 * u(1) - 1.0;
    v1(2) = u(1) - 2.0 * u(0) - 1.0;

    v2 = u.segment<3>(2);

    double c = u(5);

    bias_ = - v2.cwiseQuotient(v1);
    gain_ = (- v1 / (bias_.dot(v2) + c)).cwiseSqrt();
}

gain_bias_3axis::gain_bias_3axis(unsigned num_samples)
    : num_samples_(num_samples)
{
    reset();
}

state gain_bias_3axis::add_sample(double x, double y, double z)
{
    if (current_state_ == state::GETTING_SAMPLES)
    {
        samples_x_.push_back(x);
        samples_y_.push_back(y);
        samples_z_.push_back(z);

        if (samples_x_.size() == num_samples_)
            current_state_ = state::NEXT_AXIS;
    }

    return current_state_;
}

state gain_bias_3axis::axis_finished()
{
    if (current_state_ == state::NEXT_AXIS)
    {
        apply_median_filter();

        // Calculate variance
        axis_variance_(0) += variance(samples_x_);
        axis_variance_(1) += variance(samples_y_);
        axis_variance_(2) += variance(samples_z_);

        // Calculate means
        axis_means_(current_axis_, 0) = mean(samples_x_);
        axis_means_(current_axis_, 1) = mean(samples_y_);
        axis_means_(current_axis_, 2) = mean(samples_z_);

        // Reset vectors
        samples_x_.clear();
        samples_y_.clear();
        samples_z_.clear();

        samples_x_.reserve(num_samples_);
        samples_y_.reserve(num_samples_);
        samples_z_.reserve(num_samples_);

        current_axis_++;

        if (current_axis_ == 6)
        {
            current_state_ = state::DONE;

            estimate();
        }
        else
            current_state_ = state::GETTING_SAMPLES;
    }

    return current_state_;
}

int gain_bias_3axis::get_axis_percent_done()
{
    return (samples_x_.size() * 100 + num_samples_ / 2) / num_samples_;
}

Eigen::Vector3d gain_bias_3axis::get_gain_result()
{
    return gain_;
}

Eigen::Vector3d gain_bias_3axis::get_bias_result()
{
    return bias_;
}

Eigen::Vector3d gain_bias_3axis::get_variance()
{
    return axis_variance_ / 6.0;
}

void gain_bias_3axis::reset()
{
    samples_x_.clear();
    samples_y_.clear();
    samples_z_.clear();

    samples_x_.reserve(num_samples_);
    samples_y_.reserve(num_samples_);
    samples_z_.reserve(num_samples_);

    current_axis_ = 0;
    current_state_ = state::GETTING_SAMPLES;

    axis_variance_.setZero();
    axis_means_.setZero();
}

bool gain_bias_3axis::valid()
{
    return current_state_ == state::DONE;
}

} // end calibration
