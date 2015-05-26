class histogram {
private:
	const float minimum_;
	const float maximum_;
	const float bin_size_;
	std::vector<float> bins_;

public:
	histogram(float mn, float mx, float sz) :
		minimum_(mn), maximum_(mx), bin_size_(sz), bins_(number_of_bins(), 0) { }
		
	std::size_t number_of_bins() const {
		return std::ceil((maximum_ - minimum_) / bin_size_);
	}
		
	std::ptrdiff_t bin_for_value(float val) const {
		return std::floor((val - minimum_) / bin_size_);
	}
	
	float value_for_bin(std::ptrdiff_t i) const {
		return minimum_ + (i * bin_size_);
	}
	
	void add(float val) {
		std::ptrdiff_t i = bin_for_value(val);
		if(i >= 0 && i < bins_.size()) bins_[i] += 1.0;
	}
	
	void normalize() {
		float s = sum();
		for(float& v : bins_) v /= s;
	}
	
	float operator[](std::ptrdiff_t i) const {
		if(i < 0 || i >= bins_.size()) return 0.0;
		else return bins_[i];
	}
	
	float sum(std::ptrdiff_t mn = 0, std::ptrdiff_t mx = -1) const {
		if(mx == -1) mx = bins_.size();
		float s = 0.0;
		for(std::ptrdiff_t i = mn; i < mx; ++i) s += bins_[i];
		return s;
	}
	
	float entropy(std::ptrdiff_t mn = 0, std::ptrdiff_t mx = -1) const {
		if(mx == -1) mx = bins_.size();
		float e = 0;
		float s = sum(mn, mx);
		if(s == 0.0) return 0.0;
		for(std::ptrdiff_t i = mn; i < mx; ++i) {
			if(bins_[i] == 0.0) continue;
			float v = bins_[i] / s;
			e -= v * std::log2(v);
		}
		return e;
	}
	
	std::ptrdiff_t maximal_entropy_threshold() const {
		std::ptrdiff_t i_max;
		float e_max = -INFINITY;
		for(std::ptrdiff_t i = 0; i < bins_.size(); ++i) {
			float e = entropy(0, i) + entropy(i);
			if(e > e_max) {
				e_max = e;
				i_max = i;
			}
		}
		return i_max;
	}
	
	float difference(const histogram& h, float offset = 0.0) const {
		float sz = std::min(bin_size_, h.bin_size_);
		float mn = std::min(minimum_, h.minimum_ + offset);
		float mx = std::max(maximum_, h.maximum_ + offset);
		
		float diff = 0.0;
		for(float v = mn; v < mx; v += sz) {
			std::ptrdiff_t i1 = bin_for_value(v);
			std::ptrdiff_t i2 = h.bin_for_value(v + offset);
			float d = (*this)[i1] - h[i2];
			diff += d*d;
		}
		
		return diff;
	}
	
	void export_to_file(const std::string& file) const {
		std::ofstream out(file);
		for(std::ptrdiff_t i = 0; i < bins_.size(); ++i) {
			out << value_for_bin(i) << ' ' << bins_[i] << std::endl;
		}
	}
};
