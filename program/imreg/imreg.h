#include <Eigen/Eigen>
#include <string>

namespace pcf {

Eigen::Affine2f affine_image_registration(const std::string& fixed, const std::string& loose);

}