#ifndef LIBPOINTSCAN_CALC_H
#define LIBPOINTSCAN_CALC_H

#ifdef __cplusplus

#include "enums.h"
#include "NormalEstimator/NormalEstimator.h"

#include <opencv2/core/core.hpp>

namespace ps {

    /** \brief Computes the normal vector for the complete cloud.
      * \param[in]  obj_vec the array of object points
      * \param[out] nor_vec the output array of normal vectors
      * \param[in]  cnt_pts the viewpoint
      * \param[in]  hgt the height of the object map
      * \param[in]  wdt the width of the object map
      * \param[in]  nor_mtd the normal estimation method
      * \param[in]  brd_mtd the policy for handling borders
      * \param[in]  sys_mtd the policy for handling coordinate system
      * \param[in]  dcf the max depth change factor
      * \param[in]  nss the normal smoothing size
      * \param[in]  dds the policy for using depth dependent smoothing
      */
    void calcNormalVectors(std::vector<cv::Point3f> obj_vec, std::vector<cv::Point3f> &nor_vec, cv::Point3f cnt_pts, int hgt, int wdt,
                           int nor_mtd = AVERAGE_3D_GRADIENT, int brd_mtd = BORDER_POLICY_IGNORE, int sys_mtd = SYSTEM_CARTESIAN, float dcf = 5.0f, float nss = 5.0f, bool dds = true);
}

#endif // __cplusplus

#endif // LIBPOINTSCAN_CALC_H
