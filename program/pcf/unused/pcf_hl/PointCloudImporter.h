#ifndef PCFHL_POINT_CLOUD_IMPORTER_H_
#define PCFHL_POINT_CLOUD_IMPORTER_H_

#include "SourceUnit.h"
#include "../pcf/point_cloud/unorganized_point_cloud.h"

namespace pcf {

class PointCloudImporter : public SourceUnit {
private:
	std::string file_name_;

public:
	void set_file_name(const std::string& nm) { file_name_ = nm; }
	const std::string& get_file_name() const { return file_name_; }

	virtual void run() override;
};


}

#endif