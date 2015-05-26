#ifndef PCF_MASKED_IMAGE_H_
#define PCF_MASKED_IMAGE_H_

#include "image.h"

namespace pcf {

template<typename T, int Type>
class masked_image : public image<T, Type> {
	using super = image<T, Type>;

protected:
	cv::Mat mask_;

public:
	masked_image(std::size_t w, std::size_t h);
	masked_image(const super&);
	masked_image(const masked_image&);

	masked_image& operator=(const super&);
	masked_image& operator=(const masked_image&);
	
	bool valid(const index_2dim& ind) const { return mask_.at<bool>(ind[0], ind[1]); }
	void validate(const index_2dim& ind) { mask_.at<bool>(ind[0], ind[1]) = true; }
	void invalidate(const index_2dim& ind) { mask_.at<bool>(ind[0], ind[1]) = false; }
		
	std::pair<T, T> minimum_and_maximum() const;
	void export_visualization_to_image_file(const std::string&) const;
	void normalize(T mn, T mx);

	void apply_transformation(const Eigen::Affine2f&);
};

using masked_intensity_image = image<float, CV_32FC1>;

}

#include "masked_image.h"

#endif
