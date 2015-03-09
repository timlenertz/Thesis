#include "calc.h"
#include "enums.h"

#include <opencv2/core/core_c.h>

namespace ps {

void calcNormalVectors(std::vector<cv::Point3f> obj_vec, std::vector<cv::Point3f> &nor_vec, cv::Point3f cnt_pts,
                       int hgt, int wdt, int nor_mtd, int brd_mtd, int sys_mtd, float dcf, float nss, bool dds)
{
    NormalEstimator nor_est;
    nor_est.setNormalMethod(nor_mtd);
    nor_est.setBorderPolicy(brd_mtd);
    nor_est.setSystemMethod(sys_mtd);
    nor_est.setMaxDepthChangeFactor(dcf);
    nor_est.setNormalSmoothingSize(nss);
    nor_est.setUseDepthDependentSmoothing(dds);
    nor_est.setViewPoint(cnt_pts.x, cnt_pts.y, cnt_pts.z);
    nor_est.setInputCloud(obj_vec, hgt, wdt);
    nor_est.compute(nor_vec);
}

}
