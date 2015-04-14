#include "NormalEstimator.h"

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"

namespace ps {
NormalEstimator::~NormalEstimator()
{
    clearData();
}

void NormalEstimator::initData()
{
    if(brd_mtd != BORDER_POLICY_IGNORE &&
       brd_mtd != BORDER_POLICY_MIRROR)
        return;//ERROR Unknown border policy

    if(nor_mtd != AVERAGE_3D_GRADIENT  &&
       nor_mtd != AVERAGE_DEPTH_CHANGE &&
       nor_mtd != COVARIANCE_MATRIX    &&
       nor_mtd != SIMPLE_3D_GRADIENT)
        return;//ERROR Unknown normal estimation method

    clearData();

    // compute derivatives
    if(nor_mtd == AVERAGE_3D_GRADIENT)
        initAverage3DGradientMethod();
    else if(nor_mtd == AVERAGE_DEPTH_CHANGE)
        initAverageDepthChangeMethod();
    else if(nor_mtd == COVARIANCE_MATRIX)
        initCovarianceMatrixMethod();
    else if(nor_mtd == SIMPLE_3D_GRADIENT)
        initSimple3DGradientMethod();
}
void NormalEstimator::clearData()
{
    if(dif_x != 0)
        delete[] dif_x;
    if(dif_y != 0)
        delete[] dif_y;
    if(dst_map != 0)
        delete[] dst_map;

    dif_x = 0;
    dif_y = 0;
    dst_map = 0;
}

void NormalEstimator::initAverage3DGradientMethod()
{
    int data_size =(((int)inp.size()) << 2);
    dif_x = new float[data_size];
    dif_y = new float[data_size];
    memset(dif_x, 0, sizeof(float) * data_size);
    memset(dif_y, 0, sizeof(float) * data_size);

    // x u x
    // l x r
    // x d x
    cv::Point3f* point_up = &(inp[1]);
    cv::Point3f* point_dn = point_up + (wdt << 1);
    cv::Point3f* point_lf = &(inp[wdt]);
    cv::Point3f* point_rg = point_lf + 2;

    float* diff_x_ptr = dif_x +((wdt + 1) << 2);
    float* diff_y_ptr = dif_y +((wdt + 1) << 2);
    int    diff_skip = 8; //Skip last element in row and the first in the next row

    for(int row_i = 1 ; row_i < hgt - 1 ; row_i++)
    {
        for(int col_i = 0 ; col_i < wdt - 2 ; col_i++)
        {
            diff_x_ptr[0] = point_rg[col_i].x - point_lf[col_i].x;
            diff_x_ptr[1] = point_rg[col_i].y - point_lf[col_i].y;
            diff_x_ptr[2] = point_rg[col_i].z - point_lf[col_i].z;
            diff_x_ptr += 4;

            diff_y_ptr[0] = point_dn[col_i].x - point_up[col_i].x;
            diff_y_ptr[1] = point_dn[col_i].y - point_up[col_i].y;
            diff_y_ptr[2] = point_dn[col_i].z - point_up[col_i].z;
            diff_y_ptr += 4;
        }

        point_up += wdt;
        point_dn += wdt;
        point_lf += wdt;
        point_rg += wdt;
        diff_x_ptr += diff_skip;
        diff_y_ptr += diff_skip;
    }

    // Compute integral images
    iim_dx.setInput(dif_x, wdt, hgt, 4, wdt << 2);
    iim_dy.setInput(dif_y, wdt, hgt, 4, wdt << 2);

    init_Average3DGradient  = true;
    init_AverageDepthChange = false;
    init_CovarianceMatrix    = false;
    init_Simple3DGradient   = false;
}
void NormalEstimator::initAverageDepthChangeMethod()
{
    // number of entries per element(equal or bigger than dimensions)
    int srd_ele = sizeof(cv::Point3f) / sizeof(float);
    // number of entries per row(equal or bigger than element_stride number of elements per row)
    int srd_row = srd_ele * wdt;

    const float* dat = reinterpret_cast<const float*>(&inp.at(0));

    // integral image over the z - value
    iim_dpt.setInput(&(dat[2]), wdt, hgt, srd_ele, srd_row);

    init_Average3DGradient  = false;
    init_AverageDepthChange = true;
    init_CovarianceMatrix    = false;
    init_Simple3DGradient   = false;
}
void NormalEstimator::initCovarianceMatrixMethod()
{
    // number of entries per element(equal or bigger than dimensions)
    int srd_ele = sizeof(cv::Point3f) / sizeof(float);
    // number of entries per row(equal or bigger than element_stride number of elements per row)
    int srd_row = srd_ele * wdt;

    const float* dat = reinterpret_cast<const float*>(&inp.at(0));

    iim_xyz.setSecondOrderComputation(true);
    iim_xyz.setInput(dat, wdt, hgt, srd_ele, srd_row);

    init_Average3DGradient  = false;
    init_AverageDepthChange = false;
    init_CovarianceMatrix    = true;
    init_Simple3DGradient   = false;
}
void NormalEstimator::initSimple3DGradientMethod()
{
    // number of entries per element(equal or bigger than dimensions)
    int srd_ele = sizeof(cv::Point3f) / sizeof(float);
    // number of entries per row(equal or bigger than element_stride number of elements per row)
    int srd_row = srd_ele * wdt;

    const float* dat = reinterpret_cast<const float*>(&inp.at(0));

    iim_xyz.setSecondOrderComputation(false);
    iim_xyz.setInput(dat, wdt, hgt, srd_ele, srd_row);

    init_Average3DGradient  = false;
    init_AverageDepthChange = false;
    init_CovarianceMatrix    = false;
    init_Simple3DGradient   = true;
}

void NormalEstimator::compute(std::vector<cv::Point3f> &out)
{
    if(inp.empty() || int(inp.size()) != hgt * wdt)
    {
        out.clear();
        return;
    }

    // Resize the output dataset
    if(!out.empty())
    {
        out.clear();
    }

    if(sys_mtd == SYSTEM_CARTESIAN)
    {
        out.resize(int(inp.size()));
        computeFeature(out);
    }
    else if(sys_mtd == SYSTEM_CYLINDRICAL)
    {
        out.resize(out_hgt * out_wdt);

        std::vector<cv::Point3f> tmp_out(int(inp.size()));
        computeFeature(tmp_out);

        int brd = int(5 * nss);
        cv::Point3f* out_blk = &(tmp_out[brd * out_wdt]);
        for(int i = 0 ; i < out_hgt * out_wdt ; i++)
        {
            out[i].x = out_blk[i].x;
            out[i].y = out_blk[i].y;
            out[i].z = out_blk[i].z;
        }
    }
}
void NormalEstimator::computeFeature(std::vector<cv::Point3f> &out)
{
    // Compute depth-change map
    unsigned char* depthChangeMap = new unsigned char[inp.size()];
    memset(depthChangeMap, 255, inp.size());

    if(dst_map != 0)
        delete[] dst_map;
    dst_map = new float[int(inp.size())];
    float* distanceMap = dst_map;

    for(int row_idx = 0 ; row_idx < hgt - 1 ; row_idx++)
    {
        for(int col_idx = 0 ; col_idx < wdt - 1 ; col_idx++)
        {
            register int idx = row_idx * wdt + col_idx;

            register float depth  = inp[idx].z;
            register float depthR = inp[idx + 1].z;
            register float depthD = inp[idx + wdt].z;

            /*if(!isFinite(depth))
                depthChangeMap[idx] = 0;
            continue;*/

            register float depthDependendDepthChange = dcf * (fabsf(depth) + 1.0f) * 2.0f;

            if(fabs(depth - depthR) > depthDependendDepthChange || !isFinite(depth) || !isFinite(depthR))
            {
                depthChangeMap[idx] = 0;
                depthChangeMap[idx+1] = 0;
                distanceMap[idx] = 0.0f;
            }
            if(fabs(depth - depthD) > depthDependendDepthChange || !isFinite(depth) || !isFinite(depthD))
            {
                depthChangeMap[idx] = 0;
                depthChangeMap[idx + wdt] = 0;
                distanceMap[idx] = 0.0f;
            }

            if(isFinite(depth) && isFinite(depthR) && isFinite(depthD)){
                register float valX_h  = inp[idx].x - inp[idx + 1].x;
                register float valY_h  = inp[idx].y - inp[idx + 1].y;
                register float valZ_h  = inp[idx].z - inp[idx + 1].z;
                register float valX_w  = inp[idx].x - inp[idx + wdt].x;
                register float valY_w  = inp[idx].y - inp[idx + wdt].y;
                register float valZ_w  = inp[idx].z - inp[idx + wdt].z;

                register float dh = sqrt(valX_h * valX_h + valY_h * valY_h + valZ_h * valZ_h);
                register float dw = sqrt(valX_w * valX_w + valY_w * valY_w + valZ_w * valZ_w);

                float area = sqrt(dh * dw);
                if(area > 0.0f)
                {
                    area = 0.1f / area;

                //std::cout << area << std::endl;
                    distanceMap[idx] = area;
                }

            }
            else
                distanceMap[idx] = 0.0f;

        }
    }


    exportNormalize(cv::Mat(1, wdt*hgt, CV_32FC1, distanceMap).reshape(0, hgt),"dst_1.png");

    cv::Mat brd = cv::Mat(1, wdt*hgt, CV_32FC1, distanceMap).reshape(0, hgt);



    cv::medianBlur(brd, brd, 5);

    exportNormalize(brd,"dst_med.png");
    cv::medianBlur(brd, brd, 5);

    exportNormalize(brd,"dst_med2.png");

    brd = brd.reshape(0, wdt*hgt);

    std::vector<float> brd_vec;
    brd.copyTo(brd_vec);

    distanceMap = reinterpret_cast<float*>(&brd_vec.at(0));

    //distanceMap

    // Compute distance map
//    if(dst_map != 0)
//        delete[] dst_map;
//    dst_map = new float[int(inp.size())];
//    float* distanceMap = dst_map;
//    for(int idx = 0 ; idx < int(inp.size()) ; idx++)
//    {
//        if(depthChangeMap[idx] == 0)
//            distanceMap[idx] = 0.0f;
//        else
//        {
//            /*distanceMap[idx] = std::sqrt(inp[idx].x * inp[idx].x +
//                                         inp[idx].y * inp[idx].y +
//                                         inp[idx].z * inp[idx].z);*/

//            if(!isFinite(inp[idx].x) || !isFinite(inp[idx + 1].x) || !isFinite(inp[idx + wdt].x)){


//            register float valX_h  = inp[idx].x - inp[idx + 1].x;
//            register float valY_h  = inp[idx].y - inp[idx + 1].y;
//            register float valZ_h  = inp[idx].z - inp[idx + 1].z;
//            register float valX_w  = inp[idx].x - inp[idx + wdt].x;
//            register float valY_w  = inp[idx].y - inp[idx + wdt].y;
//            register float valZ_w  = inp[idx].z - inp[idx + wdt].z;
//            float a = sqrt(valX_h * valX_h + valY_h * valY_h + valZ_h * valZ_h) *
//                      sqrt(valX_w * valX_w + valY_w * valY_w + valZ_w * valZ_w);
//            /*register float valX  = inp[idx].x + inp[idx + 1].x;
//            register float depthR = inp[idx + 1].z;
//            register float depthD = inp[idx + wdt].z;*/
//            distanceMap[idx] = a;//sqrt(0.01f / a);
//            }
//            else
//                distanceMap[idx] = 0.0f;


//            //distanceMap[idx] = static_cast<float>(wdt + hgt);
//        }
//    }



    // First pass
    float* previous_row = distanceMap;
    float* current_row  = previous_row + wdt;
    for(int row_idx = 1 ; row_idx < hgt ; row_idx++)
    {
        for(int col_idx = 1 ; col_idx < wdt ; col_idx++)
        {
            register float upLeft  = previous_row[col_idx - 1] + 1.4f;
            register float up      = previous_row[col_idx]     + 1.0f;
            register float upRight = previous_row[col_idx + 1] + 1.4f;
            register float left    = current_row[col_idx - 1]  + 1.0f;
            register float center  = current_row[col_idx];

            register float minValue = std::min(std::min(upLeft, up), std::min(left, upRight));
            if(minValue < center)
                current_row[col_idx] = minValue;
        }
        previous_row = current_row;
        current_row += wdt;
    }

    // Second pass
    float* next_row = distanceMap + wdt * (hgt - 1);
    current_row = next_row - wdt;
    for(int row_idx = hgt - 2 ; row_idx >= 0 ; row_idx--)
    {
        for(int col_idx = wdt - 2 ; col_idx >= 0 ; col_idx--)
        {
            register float lowerLeft  = next_row[col_idx - 1]    + 1.4f;
            register float lower      = next_row[col_idx]        + 1.0f;
            register float lowerRight = next_row[col_idx + 1]    + 1.4f;
            register float right      = current_row[col_idx + 1] + 1.0f;
            register float center     = current_row[col_idx];

            register float minValue = std::min(std::min(lowerLeft, lower), std::min(right, lowerRight));
            if(minValue < center)
                current_row[col_idx] = minValue;
        }
        next_row = current_row;
        current_row -= wdt;
    }

    exportNormalize(cv::Mat(1, wdt*hgt, CV_32FC1, distanceMap).reshape(0, hgt),"dst_2.png");

    //crv_map = new float[int(inp.size())]{ 0.0f };
    //memset(crv_map, 0.0f, inp.size());

    /*crv_map.resize(int(inp.size()), cv::Point3f(0.0f, 0.0f, 0.0f));

    brd_mtd = BORDER_POLICY_MIRROR;
    nor_mtd = COVARIANCE_MATRIX;*/


    float bad_point = std::numeric_limits<float>::quiet_NaN();
    if(brd_mtd == BORDER_POLICY_IGNORE)
    {
        int border = int(nss);

        // Set all normals that we do not touch to NaN
        // Top and bottom borders
        cv::Point3f* first_row = &out.at(0);
        cv::Point3f* last_row  = first_row + wdt * (hgt - border);
        for(int idx = 0 ; idx < border * wdt ; idx++)
        {
            first_row[idx].x = first_row[idx].y = first_row[idx].z = bad_point;
            last_row[idx].x  = last_row[idx].y  = last_row[idx].z  = bad_point;
        }

        // Left and right borders actually columns
        cv::Point3f* left_col  = &out.at(border * wdt);
        cv::Point3f* right_col = left_col + wdt - border;
        for(int row_idx = border ; row_idx < hgt - border ; row_idx++)
        {
            for(int col_idx = 0 ; col_idx < border ; col_idx++)
            {
                left_col[col_idx].x  = left_col[col_idx].y  = left_col[col_idx].z  = bad_point;
                right_col[col_idx].x = right_col[col_idx].y = right_col[col_idx].z = bad_point;
            }
            left_col  += wdt;
            right_col += wdt;
        }

        #pragma omp parallel default(shared)
        {
            #pragma omp for schedule(dynamic, 10000) nowait
            for(int row_idx = border ; row_idx < hgt - border ; row_idx++)
            {
                for(int col_idx = border ; col_idx < wdt - border ; col_idx++)
                {
                    register int idx = row_idx * wdt + col_idx;

                    register float depth = inp[idx].z;
                    if(!isFinite(depth))
                    {
                        out[idx].x = out[idx].y = out[idx].z = bad_point;
                        continue;
                    }

                    float smoothing = (std::min)(distanceMap[idx], 25.0f);
                    /*if(use)
                    {
                        smoothing = (std::min)(distanceMap[idx], nss + static_cast<float>(depth)/10.0f);
                    }
                    else
                    {
                        smoothing = (std::min)(distanceMap[idx], nss);
                    }*/

                    if(smoothing > 2.0f)
                    {
                        setRectSize(static_cast<int>(smoothing), static_cast<int>(smoothing));
                        computePointNormal(col_idx, row_idx, idx, out[idx]);
                    }
                    else
                    {
                        out[idx].x = out[idx].y = out[idx].z = bad_point;
                    }
                }
            }
        }
    }
    else if(brd_mtd == BORDER_POLICY_MIRROR)
    {
        #pragma omp parallel default(shared)
        {
            #pragma omp for schedule(dynamic, 10000) nowait
            for(int row_idx = 0 ; row_idx < hgt ; row_idx++)
            {
                for(int col_idx = 0 ; col_idx < wdt ; col_idx++)
                {
                    register int idx = row_idx * wdt + col_idx;

                    register float depth = inp[idx].z;
                    if(!isFinite(depth))
                    {
                        out[idx].x = bad_point;
                        out[idx].y = bad_point;
                        out[idx].z = bad_point;
                        continue;
                    }

                    float smoothing;
                    if(use)
                    {
                        smoothing = (std::min)(distanceMap[idx], nss + static_cast<float>(depth)/10.0f);
                    }
                    else
                    {
                        smoothing = (std::min)(distanceMap[idx], nss);
                    }

                    if(smoothing > 2.0f)
                    {
                        setRectSize(static_cast<int>(smoothing), static_cast<int>(smoothing));
                        computePointNormalMirror(col_idx, row_idx, idx, out[idx]);
                    }
                    else
                    {
                        out[idx].x = bad_point;
                        out[idx].y = bad_point;
                        out[idx].z = bad_point;
                    }
                }
            }
        }
    }
    delete[] depthChangeMap;


    /*cv::Mat cc = cv::Mat(crv_map).reshape(0, hgt);
    std::vector<cv::Mat> lay_vec;
    cv::split(cc, lay_vec);

    exportNormalize(lay_vec[0],"crv_0.png");
    exportNormalize(lay_vec[1],"crv_1.png");
    exportNormalize(lay_vec[2],"crv_2.png");*/
}

void NormalEstimator::computePointNormal(const int pos_x, const int pos_y, const int point_index, cv::Point3f &normal)
{
    float bad_point = std::numeric_limits<float>::quiet_NaN();
    if(nor_mtd == AVERAGE_3D_GRADIENT)
    {
        if(!init_Average3DGradient)
            initAverage3DGradientMethod();

        unsigned count_x = iim_dx.getFiniteElementsCount(pos_x - rec_wdt_2, pos_y - rec_hgt_2, rec_wdt, rec_hgt);
        unsigned count_y = iim_dy.getFiniteElementsCount(pos_x - rec_wdt_2, pos_y - rec_hgt_2, rec_wdt, rec_hgt);
        if(count_x == 0 || count_y == 0)
        {
            normal.x = normal.y = normal.z = bad_point;
            return;
        }

        cv::Vec3d gradient_x = iim_dx.getFirstOrderSum(pos_x - rec_wdt_2, pos_y - rec_hgt_2, rec_wdt, rec_hgt);
        cv::Vec3d gradient_y = iim_dy.getFirstOrderSum(pos_x - rec_wdt_2, pos_y - rec_hgt_2, rec_wdt, rec_hgt);
        cv::Vec3d normal_vector = gradient_y.cross(gradient_x);
        double normal_length = normal_vector.ddot(normal_vector);
        if(normal_length == 0.0f)
        {
            normal.x = normal.y = normal.z = bad_point;
            return;
        }
        normal_vector /= sqrt(normal_length);

        float nx = static_cast<float>(normal_vector[0]);
        float ny = static_cast<float>(normal_vector[1]);
        float nz = static_cast<float>(normal_vector[2]);

        flipNormal(inp[point_index], vpx, vpy, vpz, nx, ny, nz);

        normal.x = nx;
        normal.y = ny;
        normal.z = nz;
        return;
    }
    else if(nor_mtd == AVERAGE_DEPTH_CHANGE)
    {
        if(!init_AverageDepthChange)
            initAverageDepthChangeMethod();

        // width and height are at least 3 x 3
        unsigned count_L_z = iim_dpt.getFiniteElementsCount(pos_x - rec_wdt_2, pos_y - rec_hgt_4, rec_wdt_2, rec_hgt_2);
        unsigned count_R_z = iim_dpt.getFiniteElementsCount(pos_x + 1            , pos_y - rec_hgt_4, rec_wdt_2, rec_hgt_2);
        unsigned count_U_z = iim_dpt.getFiniteElementsCount(pos_x - rec_wdt_4, pos_y - rec_hgt_2, rec_wdt_2, rec_hgt_2);
        unsigned count_D_z = iim_dpt.getFiniteElementsCount(pos_x - rec_wdt_4, pos_y + 1             , rec_wdt_2, rec_hgt_2);

        if(count_L_z == 0 || count_R_z == 0 || count_U_z == 0 || count_D_z == 0)
        {
            normal.x = normal.y = normal.z = bad_point;
            return;
        }

        float mean_L_z = static_cast<float>(iim_dpt.getFirstOrderSum(pos_x - rec_wdt_2, pos_y - rec_hgt_4, rec_wdt_2, rec_hgt_2) / count_L_z);
        float mean_R_z = static_cast<float>(iim_dpt.getFirstOrderSum(pos_x + 1            , pos_y - rec_hgt_4, rec_wdt_2, rec_hgt_2) / count_R_z);
        float mean_U_z = static_cast<float>(iim_dpt.getFirstOrderSum(pos_x - rec_wdt_4, pos_y - rec_hgt_2, rec_wdt_2, rec_hgt_2) / count_U_z);
        float mean_D_z = static_cast<float>(iim_dpt.getFirstOrderSum(pos_x - rec_wdt_4, pos_y + 1             , rec_wdt_2, rec_hgt_2) / count_D_z);

        cv::Point3f pointL = inp[point_index - rec_wdt_4 - 1];
        cv::Point3f pointR = inp[point_index + rec_wdt_4 + 1];
        cv::Point3f pointU = inp[point_index - rec_hgt_4 * wdt - 1];
        cv::Point3f pointD = inp[point_index + rec_hgt_4 * wdt + 1];

        const float mean_x_z = mean_R_z - mean_L_z;
        const float mean_y_z = mean_D_z - mean_U_z;

        const float mean_x_x = pointR.x - pointL.x;
        const float mean_x_y = pointR.y - pointL.y;
        const float mean_y_x = pointD.x - pointU.x;
        const float mean_y_y = pointD.y - pointU.y;

        float nx = mean_x_y * mean_y_z - mean_x_z * mean_y_y;
        float ny = mean_x_z * mean_y_x - mean_x_x * mean_y_z;
        float nz = mean_x_x * mean_y_y - mean_x_y * mean_y_x;

        const float normal_length = (nx * nx + ny * ny + nz * nz);

        if(normal_length == 0.0f)
        {
            normal.x = normal.y = normal.z = bad_point;
            return;
        }

        flipNormal(inp[point_index], vpx, vpy, vpz, nx, ny, nz);

        float scale = 1.0f / sqrt(normal_length);
        normal.x = nx * scale;
        normal.y = ny * scale;
        normal.z = nz * scale;
        return;
    }
    else if(nor_mtd == COVARIANCE_MATRIX)
    {
        if(!init_CovarianceMatrix)
            initCovarianceMatrixMethod();

        unsigned count = iim_xyz.getFiniteElementsCount(pos_x - rec_wdt_2, pos_y - rec_hgt_2, rec_wdt, rec_hgt);
        if(count == 0)
        {
            normal.x = normal.y = normal.z = bad_point;
            return;
        }

        IntegralImage2D<float, 3>::FirstType  fo_elements;
        IntegralImage2D<float, 3>::SecondType so_elements;
        fo_elements = iim_xyz.getFirstOrderSum(pos_x - rec_wdt_2, pos_y - rec_hgt_2, rec_wdt, rec_hgt);
        so_elements = iim_xyz.getSecondOrderSum(pos_x - rec_wdt_2, pos_y - rec_hgt_2, rec_wdt, rec_hgt);

        cv::Vec3f center(float(fo_elements.val[0]), float(fo_elements.val[1]), float(fo_elements.val[2]));
        cv::Matx<float, 3, 3> covariance_matrix;

        covariance_matrix.val[0] = static_cast<float>(so_elements[0]);
        covariance_matrix.val[1] = covariance_matrix.val[3] = static_cast<float>(so_elements[1]);
        covariance_matrix.val[2] = covariance_matrix.val[6] = static_cast<float>(so_elements[2]);
        covariance_matrix.val[4] = static_cast<float>(so_elements[3]);
        covariance_matrix.val[5] = covariance_matrix.val[7] = static_cast<float>(so_elements[4]);
        covariance_matrix.val[8] = static_cast<float>(so_elements[5]);
        covariance_matrix -= (center / static_cast<float>(count) * center.t());

        cv::Mat eigen_value, eigen_vector;
        cv::eigen(covariance_matrix, eigen_value, eigen_vector);

        int pos = eigen_value.size().height - 1;
        float nx = eigen_vector.at<float>(pos, 0);
        float ny = eigen_vector.at<float>(pos, 1);
        float nz = eigen_vector.at<float>(pos, 2);



        flipNormal(inp[point_index], vpx, vpy, vpz, nx, ny, nz);

        normal.x = nx;
        normal.y = ny;
        normal.z = nz;
        return;
    }
    else if(nor_mtd == SIMPLE_3D_GRADIENT)
    {
        // This method does not work if lots of NaNs are in the neighborhood of the point
        if(!init_Simple3DGradient)
            initSimple3DGradientMethod();

        cv::Vec3d gradient_x = iim_xyz.getFirstOrderSum(pos_x + rec_wdt_2, pos_y - rec_hgt_2, 1, rec_hgt) -
                               iim_xyz.getFirstOrderSum(pos_x - rec_wdt_2, pos_y - rec_hgt_2, 1, rec_hgt);
        cv::Vec3d gradient_y = iim_xyz.getFirstOrderSum(pos_x - rec_wdt_2, pos_y + rec_hgt_2, rec_wdt, 1) -
                               iim_xyz.getFirstOrderSum(pos_x - rec_wdt_2, pos_y - rec_hgt_2, rec_wdt, 1);
        cv::Vec3d normal_vector = gradient_y.cross(gradient_x);
        double normal_length = normal_vector.ddot(normal_vector);
        if(normal_length == 0.0f)
        {
            normal.x = normal.y = normal.z = bad_point;
            return;
        }
        normal_vector /= sqrt(normal_length);

        float nx = static_cast<float>(normal_vector[0]);
        float ny = static_cast<float>(normal_vector[1]);
        float nz = static_cast<float>(normal_vector[2]);

        flipNormal(inp[point_index], vpx, vpy, vpz, nx, ny, nz);

        normal.x = nx;
        normal.y = ny;
        normal.z = nz;
        return;
    }

    normal.x = normal.y = normal.z = bad_point;
}
void NormalEstimator::computePointNormalMirror(const int pos_x, const int pos_y, const unsigned point_index, cv::Point3f &normal)
{
//    Function<IntegralImage2D<float, 3>::FirstType, 3>::FctPoint ptr = &IntegralImage2D<float, 3>::getFirstOrderSumSE;
//    IntegralImage2D<float, 3>::FirstType  fo_elements;
//    sumArea<IntegralImage2D<float, 3>::FirstType, 3>(1, 2, 3, 4, 5, 6, ptr, &integral_image_XYZ_, fo_elements);
//    sumArea<IntegralImage2D<float, 3>::FirstType, 3>(1, 2, 3, 4, 5, 6, &IntegralImage2D<float, 3>::getFirstOrderSumSE, &integral_image_XYZ_, fo_elements);

//    Function<unsigned, 3>::FctPoint ptr2 = &IntegralImage2D<float, 3>::getFiniteElementsCountSE;
//    unsigned val;
//    sumArea<unsigned, 3>(1, 2, 3, 4, 5, 6, ptr2, &integral_image_XYZ_, val);
//    sumArea<unsigned, 3>(1, 2, 3, 4, 5, 6, &IntegralImage2D<float, 3>::getFiniteElementsCountSE, &integral_image_XYZ_, val);

    float bad_point = std::numeric_limits<float>::quiet_NaN();

    const int width = wdt;
    const int height = hgt;

    if(nor_mtd == AVERAGE_3D_GRADIENT)
    {
        if(!init_Average3DGradient)
            initAverage3DGradientMethod();

        const int start_x = pos_x - rec_wdt_2;
        const int start_y = pos_y - rec_hgt_2;
        const int end_x = start_x + rec_wdt;
        const int end_y = start_y + rec_hgt;

        unsigned count_x = 0;
        unsigned count_y = 0;
        sumArea<unsigned, 3>(start_x, start_y, end_x, end_y, width, height, &IntegralImage2D<float, 3>::getFiniteElementsCountSE, &iim_dx, count_x);
        sumArea<unsigned, 3>(start_x, start_y, end_x, end_y, width, height, &IntegralImage2D<float, 3>::getFiniteElementsCountSE, &iim_dy, count_y);
        if(count_x == 0 || count_y == 0)
        {
            normal.x = normal.y = normal.z = bad_point;
            return;
        }

        IntegralImage2D<float, 3>::FirstType gradient_x(0.0f, 0.0f, 0.0f);
        IntegralImage2D<float, 3>::FirstType gradient_y(0.0f, 0.0f, 0.0f);
        sumArea<IntegralImage2D<float, 3>::FirstType, 3>(start_x, start_y, end_x, end_y, width, height, &IntegralImage2D<float, 3>::getFirstOrderSumSE, &iim_dx, gradient_x);
        sumArea<IntegralImage2D<float, 3>::FirstType, 3>(start_x, start_y, end_x, end_y, width, height, &IntegralImage2D<float, 3>::getFirstOrderSumSE, &iim_dy, gradient_y);

        cv::Vec3f normal_vector = cv::Vec3d(gradient_x.val[0], gradient_x.val[1], gradient_x.val[2]).cross(cv::Vec3d(gradient_y.val[0], gradient_y.val[1], gradient_y.val[2]));
        double normal_length = normal_vector.ddot(normal_vector);
        if(normal_length == 0.0f)
        {
            normal.x = normal.y = normal.z = bad_point;
            return;
        }

        normal_vector /= sqrt(normal_length);

        float nx = static_cast<float>(normal_vector[0]);
        float ny = static_cast<float>(normal_vector[1]);
        float nz = static_cast<float>(normal_vector[2]);

        flipNormal(inp[point_index], vpx, vpy, vpz, nx, ny, nz);

        normal.x = nx;
        normal.y = ny;
        normal.z = nz;
        return;
    }
    else if(nor_mtd == AVERAGE_DEPTH_CHANGE)
    {
        if(!init_AverageDepthChange)
            initAverageDepthChangeMethod();

        int point_index_L_x = pos_x - rec_wdt_4 - 1;
        int point_index_L_y = pos_y;
        int point_index_R_x = pos_x + rec_wdt_4 + 1;
        int point_index_R_y = pos_y;
        int point_index_U_x = pos_x - 1;
        int point_index_U_y = pos_y - rec_hgt_4;
        int point_index_D_x = pos_x + 1;
        int point_index_D_y = pos_y + rec_hgt_4;

        if(point_index_L_x < 0)
            point_index_L_x = -point_index_L_x;
        if(point_index_U_x < 0)
            point_index_U_x = -point_index_U_x;
        if(point_index_U_y < 0)
            point_index_U_y = -point_index_U_y;
        if(point_index_R_x >= width)
            point_index_R_x = width - (point_index_R_x - (width-1));
        if(point_index_D_x >= width)
            point_index_D_x = width - (point_index_D_x - (width-1));
        if(point_index_D_y >= height)
            point_index_D_y = height - (point_index_D_y - (height-1));

        const int start_x_L = pos_x - rec_wdt_2;
        const int start_y_L = pos_y - rec_hgt_4;
        const int end_x_L = start_x_L + rec_wdt_2;
        const int end_y_L = start_y_L + rec_hgt_2;

        const int start_x_R = pos_x + 1;
        const int start_y_R = pos_y - rec_hgt_4;
        const int end_x_R = start_x_R + rec_wdt_2;
        const int end_y_R = start_y_R + rec_hgt_2;

        const int start_x_U = pos_x - rec_wdt_4;
        const int start_y_U = pos_y - rec_hgt_2;
        const int end_x_U = start_x_U + rec_wdt_2;
        const int end_y_U = start_y_U + rec_hgt_2;

        const int start_x_D = pos_x - rec_wdt_4;
        const int start_y_D = pos_y + 1;
        const int end_x_D = start_x_D + rec_wdt_2;
        const int end_y_D = start_y_D + rec_hgt_2;

        unsigned count_L_z = 0;
        unsigned count_R_z = 0;
        unsigned count_U_z = 0;
        unsigned count_D_z = 0;
        sumArea<unsigned, 1>(start_x_L, start_y_L, end_x_L, end_y_L, width, height, &IntegralImage2D<float, 1>::getFiniteElementsCountSE, &iim_dpt, count_L_z);
        sumArea<unsigned, 1>(start_x_R, start_y_R, end_x_R, end_y_R, width, height, &IntegralImage2D<float, 1>::getFiniteElementsCountSE, &iim_dpt, count_R_z);
        sumArea<unsigned, 1>(start_x_U, start_y_U, end_x_U, end_y_U, width, height, &IntegralImage2D<float, 1>::getFiniteElementsCountSE, &iim_dpt, count_U_z);
        sumArea<unsigned, 1>(start_x_D, start_y_D, end_x_D, end_y_D, width, height, &IntegralImage2D<float, 1>::getFiniteElementsCountSE, &iim_dpt, count_D_z);

        if(count_L_z == 0 || count_R_z == 0 || count_U_z == 0 || count_D_z == 0)
        {
            normal.x = normal.y = normal.z = bad_point;
            return;
        }

        double mean_L_z = 0;
        double mean_R_z = 0;
        double mean_U_z = 0;
        double mean_D_z = 0;
        sumArea<double, 1>(start_x_L, start_y_L, end_x_L, end_y_L, width, height, &IntegralImage2D<float, 1>::getFirstOrderSumSE, &iim_dpt, mean_L_z);
        sumArea<double, 1>(start_x_R, start_y_R, end_x_R, end_y_R, width, height, &IntegralImage2D<float, 1>::getFirstOrderSumSE, &iim_dpt, mean_R_z);
        sumArea<double, 1>(start_x_U, start_y_U, end_x_U, end_y_U, width, height, &IntegralImage2D<float, 1>::getFirstOrderSumSE, &iim_dpt, mean_U_z);
        sumArea<double, 1>(start_x_D, start_y_D, end_x_D, end_y_D, width, height, &IntegralImage2D<float, 1>::getFirstOrderSumSE, &iim_dpt, mean_D_z);

        mean_L_z /= double(count_L_z);
        mean_R_z /= double(count_R_z);
        mean_U_z /= double(count_U_z);
        mean_D_z /= double(count_D_z);

        cv::Point3f pointL = inp[point_index_L_y * width + point_index_L_x];
        cv::Point3f pointR = inp[point_index_R_y * width + point_index_R_x];
        cv::Point3f pointU = inp[point_index_U_y * width + point_index_U_x];
        cv::Point3f pointD = inp[point_index_D_y * width + point_index_D_x];

        const float mean_x_z = float(mean_R_z - mean_L_z);
        const float mean_y_z = float(mean_D_z - mean_U_z);

        const float mean_x_x = pointR.x - pointL.x;
        const float mean_x_y = pointR.y - pointL.y;
        const float mean_y_x = pointD.x - pointU.x;
        const float mean_y_y = pointD.y - pointU.y;

        float nx = mean_x_y * mean_y_z - mean_x_z * mean_y_y;
        float ny = mean_x_z * mean_y_x - mean_x_x * mean_y_z;
        float nz = mean_x_x * mean_y_y - mean_x_y * mean_y_x;

        const float normal_length = (nx * nx + ny * ny + nz * nz);
        if(normal_length == 0.0f)
        {
            normal.x = normal.y = normal.z = bad_point;
            return;
        }

        flipNormal(inp[point_index], vpx, vpy, vpz, nx, ny, nz);
        const float scale = 1.0f / sqrtf (normal_length);

        normal.x = nx * scale;
        normal.y = ny * scale;
        normal.z = nz * scale;
        return;
    }
    else if(nor_mtd == COVARIANCE_MATRIX)
    {
        if(!init_CovarianceMatrix)
            initCovarianceMatrixMethod();

        const int start_x = pos_x - rec_wdt_2;
        const int start_y = pos_y - rec_hgt_2;
        const int end_x = start_x + rec_wdt;
        const int end_y = start_y + rec_hgt;

        unsigned count = 0;
        sumArea<unsigned, 3>(start_x, start_y, end_x, end_y, width, height, &IntegralImage2D<float, 3>::getFiniteElementsCountSE, &iim_xyz, count);
        if(count == 0)
        {
            normal.x = normal.y = normal.z = bad_point;
            return;
        }

        cv::Vec3f center(0.0f, 0.0f, 0.0f);
        IntegralImage2D<float, 3>::FirstType  tmp_center;
        IntegralImage2D<float, 3>::SecondType so_elements;

        tmp_center[0] = 0;
        tmp_center[1] = 0;
        tmp_center[2] = 0;
        so_elements[0] = 0;
        so_elements[1] = 0;
        so_elements[2] = 0;
        so_elements[3] = 0;
        so_elements[4] = 0;
        so_elements[5] = 0;

        sumArea<IntegralImage2D<float, 3>::FirstType, 3>(start_x, start_y, end_x, end_y, width, height, &IntegralImage2D<float, 3>::getFirstOrderSumSE, &iim_xyz, tmp_center);
        sumArea<IntegralImage2D<float, 3>::SecondType, 3>(start_x, start_y, end_x, end_y, width, height, &IntegralImage2D<float, 3>::getSecondOrderSumSE, &iim_xyz, so_elements);
        center[0] = float(tmp_center[0]);
        center[1] = float(tmp_center[1]);
        center[2] = float(tmp_center[2]);

        cv::Matx<float, 3, 3> covariance_matrix;
        covariance_matrix.val[0] = static_cast<float>(so_elements[0]);
        covariance_matrix.val[1] = covariance_matrix.val[3] = static_cast<float>(so_elements[1]);
        covariance_matrix.val[2] = covariance_matrix.val[6] = static_cast<float>(so_elements[2]);
        covariance_matrix.val[4] = static_cast<float>(so_elements[3]);
        covariance_matrix.val[5] = covariance_matrix.val[7] = static_cast<float>(so_elements[4]);
        covariance_matrix.val[8] = static_cast<float>(so_elements[5]);
        covariance_matrix -= (center / static_cast<float>(count) * center.t());

        cv::Mat eigen_value, eigen_vector;
        cv::eigen(covariance_matrix, eigen_value, eigen_vector);

        int pos = eigen_value.size().height - 1;
        float nx = eigen_vector.at<float>(pos, 0);
        float ny = eigen_vector.at<float>(pos, 1);
        float nz = eigen_vector.at<float>(pos, 2);

        flipNormal(inp[point_index], vpx, vpy, vpz, nx, ny, nz);

        normal.x = nx;
        normal.y = ny;
        normal.z = nz;

        /*crv_map[point_index].x = eigen_value.at<float>(0) / (eigen_value.at<float>(0)+eigen_value.at<float>(1)+eigen_value.at<float>(2));
        crv_map[point_index].y = eigen_value.at<float>(1) / (eigen_value.at<float>(0)+eigen_value.at<float>(1)+eigen_value.at<float>(2));
        crv_map[point_index].z = eigen_value.at<float>(2) / (eigen_value.at<float>(0)+eigen_value.at<float>(1)+eigen_value.at<float>(2));*/

        return;
    }
    else if(nor_mtd == SIMPLE_3D_GRADIENT)
    {
        //ERROR "BORDER_POLICY_MIRROR not supported for normal estimation method SIMPLE_3D_GRADIENT"
    }

    normal.x = normal.y = normal.z = bad_point;
}
}
