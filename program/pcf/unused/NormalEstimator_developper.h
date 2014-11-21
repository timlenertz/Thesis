#ifndef NORMALESTIMATOR_H
#define NORMALESTIMATOR_H

#include "IntegralImage2D.h"
#include "../enums.h"
#include "../export.h"

namespace ps {

class NormalEstimator
{
    public:
        /** \brief Constructor **/
        NormalEstimator(): iim_dx(false), iim_dy(false), iim_dpt(false), iim_xyz(true)
        {
            brd_mtd = BORDER_POLICY_IGNORE;
            nor_mtd = AVERAGE_3D_GRADIENT;
            sys_mtd = SYSTEM_CARTESIAN;

            dif_x   = 0;
            dif_y   = 0;
            dst_map = 0;

            use = false;
            dcf = 20.0f * 0.001f;
            nss = 10.0f;

            init_CovarianceMatrix= false;
            init_Average3DGradient= false;
            init_Simple3DGradient = false;
            init_AverageDepthChange = false;

            rec_wdt = rec_wdt_2 = rec_wdt_4 = 0;
            rec_hgt = rec_hgt_2 = rec_hgt_4 = 0;

            vpx = vpy = vpz = 0.0f;
        }
        /** \brief Destructor **/
        virtual ~NormalEstimator();


        /** \brief Set whether to use depth depending smoothing or not
          * \param[in] in_use decides whether the smoothing is depth dependent
          */
        void setUseDepthDependentSmoothing(bool in_use){ use = in_use; }
        /** \brief The depth change threshold for computing object borders
          * \param[in] in_dcf the depth change threshold for computing object borders based on depth changes
          */
        void setMaxDepthChangeFactor(float in_dcf){ dcf = in_dcf; }
        /** \brief Set the normal smoothing size
          * \param[in] in_nss factor which influences the size of the area used to smooth normals (depth dependent if use is true)
          */
        void setNormalSmoothingSize(float in_nss){ nss = (in_nss > 0) ? in_nss : nss; }


        /** \brief Sets the policy for handling borders.
          * \param[in] in_brd_mtd the border policy.
          */
        void setBorderPolicy(int in_brd_mtd){ brd_mtd = in_brd_mtd; }
        /** \brief Set the normal estimation method.
          * \param[in] in_nor_mtd the method used for normal estimation
          */
        void setNormalMethod(int in_nor_mtd){ nor_mtd = in_nor_mtd; }
        /** \brief Set the policy for handling coordinate system.
          * \param[in] in_sys_mtd the coordinate system policy
          */
        void setSystemMethod(int in_sys_mtd){ sys_mtd = in_sys_mtd; }


        /** \brief Provide a pointer to the input dataset
          * \param[in] in_inp the 3D point vector to describe the object
          * \param[in] in_wdt the width of the depth map
          * \param[in] in_hgt the height of the depth map
          */
        void setInputCloud(const std::vector<cv::Point3f> &in_inp, int in_wdt = 0, int in_hgt = 0)
        {
            if(sys_mtd != SYSTEM_CARTESIAN     &&
               sys_mtd != SYSTEM_CYLINDRICAL)
                return;//ERROR Unknown border policy

            if(sys_mtd == SYSTEM_CARTESIAN)
            {
                inp = in_inp;
                wdt = in_wdt;
                hgt = in_hgt;
            }
            else if(sys_mtd == SYSTEM_CYLINDRICAL)
            {
                int brd = int(5 * nss);

                out_wdt = in_wdt;
                out_hgt = in_hgt;

                inp.resize((in_hgt + 2 * brd) * in_wdt);

                cv::Point3f* beg_blk = &(inp[brd * in_wdt - 1]);
                cv::Point3f* end_blk = &(inp[(in_hgt + brd) * in_wdt]);
                cv::Point3f* dat_blk = &(inp[brd * in_wdt]);

                for(int i = 0 ; i < in_wdt * in_hgt ; i++)
                {
                    dat_blk[i].x = in_inp[i].x;
                    dat_blk[i].y = in_inp[i].y;
                    dat_blk[i].z = in_inp[i].z;
                }
                for(int i = 0 ; i < in_wdt * brd ; i++)
                {
                    end_blk[i].x = in_inp[i].x;
                    end_blk[i].y = in_inp[i].y;
                    end_blk[i].z = in_inp[i].z;
                }
                for(int i = 0 ; i < in_wdt * brd ; i++)
                {
                    beg_blk[-i].x = in_inp[int(in_inp.size())-1-i].x;
                    beg_blk[-i].y = in_inp[int(in_inp.size())-1-i].y;
                    beg_blk[-i].z = in_inp[int(in_inp.size())-1-i].z;
                }

                hgt = in_hgt + 2 * brd;
                wdt = in_wdt;
            }

            init_CovarianceMatrix = init_Average3DGradient = init_AverageDepthChange = false;

            // Initialize the correct data structure based on the normal estimation method chosen
            initData();
        }
        /** \brief Set the viewpoint.
          * \param vpx the X coordinate of the viewpoint
          * \param vpy the Y coordinate of the viewpoint
          * \param vpz the Z coordinate of the viewpoint
          */
        void setViewPoint (float in_vpx, float in_vpy, float in_vpz){ vpx = in_vpx; vpy = in_vpy; vpz = in_vpz; }

        /** \brief Computes the normal for the complete cloud.
          * \param[out] out the resultant normals
          */
        void compute(std::vector<cv::Point3f> &out);



    protected:
        /** \brief Initialize the data structures, based on the normal estimation method chosen. */
        void initData();
        /** \brief Clear the data structures. */
        void clearData();



    private:
        /** \brief The policy for handling borders. */
        int brd_mtd;
        /** \brief The normal estimation method to use. Currently, 4 implementations are provided:
          * - COVARIANCE_MATRIX
          * - AVERAGE_3D_GRADIENT
          * - AVERAGE_DEPTH_CHANGE
          * - SIMPLE_3D_GRADIENT (not supported with BORDER_POLICY_MIRROR policy)
          */
        int nor_mtd;
        /** \brief The policy for handling coordinate system. */
        int sys_mtd;


        /** \brief The input depth map data. */
        std::vector<cv::Point3f> inp;
        /** \brief The input depth map height. */
        int hgt;
        /** \brief The input depth map width. */
        int wdt;
        /** \brief The output depth map height (only use in cylindrical system handling policy). */
        int out_hgt;
        /** \brief The output depth map width (only use in cylindrical system handling policy). */
        int out_wdt;


        /** derivatives in x-direction */
        float *dif_x;
        /** derivatives in y-direction */
        float *dif_y;
        /** distance map */
        float *dst_map;

        /*std::vector<cv::Point3f> crv_map;*/


        /** \brief Smooth data based on depth (true/false). */
        bool  use;
        /** \brief Threshold for detecting depth discontinuities (Max depth change factor) */
        float dcf;
        /** \brief Normal smoothing size*/
        float nss;


        /** integral image in x-direction */
        IntegralImage2D<float,3> iim_dx;
        /** integral image in y-direction */
        IntegralImage2D<float,3> iim_dy;
        /** integral image */
        IntegralImage2D<float,1> iim_dpt;
        /** integral image xyz */
        IntegralImage2D<float,3> iim_xyz;



    private:
        /** \brief Computes the normal for the complete cloud.
          * \param[out] out the resultant normals
          */
        void computeFeature(std::vector<cv::Point3f> &out);
        /** \brief Computes the normal at the specified position.
          * \param[in] pos_x x position (pixel)
          * \param[in] pos_y y position (pixel)
          * \param[in] pos_idx the position index of the point
          * \param[out] nor the output estimated normal
          */
        void computePointNormal(const int pos_x, const int pos_y, const int pos_idx, cv::Point3f &nor);
        /** \brief Computes the normal at the specified position with mirroring for border handling.
          * \param[in] pos_x x position (pixel)
          * \param[in] pos_y y position (pixel)
          * \param[in] pos_idx the position index of the point
          * \param[out] nor the output estimated normal
          */
        void computePointNormalMirror(const int pos_x, const int pos_y, const unsigned pos_idx, cv::Point3f &nor);


        /** \brief True when a dataset has been received and the average 3d gradient data has been initialized. */
        bool init_Average3DGradient;
        /** \brief True when a dataset has been received and the average depth change data has been initialized. */
        bool init_AverageDepthChange;
        /** \brief True when a dataset has been received and the covariance_matrix data has been initialized. */
        bool init_CovarianceMatrix;
        /** \brief True when a dataset has been received and the simple 3d gradient data has been initialized. */
        bool init_Simple3DGradient;
        /** \brief Internal initialization method for AVERAGE_3D_GRADIENT estimation. */
        void initAverage3DGradientMethod ();
        /** \brief Internal initialization method for AVERAGE_DEPTH_CHANGE estimation. */
        void initAverageDepthChangeMethod ();
        /** \brief Internal initialization method for COVARIANCE_MATRIX estimation. */
        void initCovarianceMatrixMethod ();
        /** \brief Internal initialization method for SIMPLE_3D_GRADIENT estimation. */
        void initSimple3DGradientMethod ();


        /** The width of the neighborhood region used for computing the normal. */
        int rec_wdt, rec_wdt_2, rec_wdt_4;
        /** The height of the neighborhood region used for computing the normal. */
        int rec_hgt, rec_hgt_2, rec_hgt_4;
        /** \brief Set the regions size which is considered for normal estimation.
          * \param[in] in_wdt the width of the search rectangle
          * \param[in] in_hgt the height of the search rectangle
          */
        void setRectSize (const int in_wdt, const int in_hgt)
        {
            rec_wdt   = in_wdt;
            rec_wdt_2 = in_wdt / 2;
            rec_wdt_4 = in_wdt / 4;
            rec_hgt   = in_hgt;
            rec_hgt_2 = in_hgt / 2;
            rec_hgt_4 = in_hgt / 4;
        }

        /** \brief Values describing the viewpoint ("pinhole" camera model assumed). By default, the viewpoint is set to 0,0,0. */
        float vpx, vpy, vpz;
        /** \brief Flip (in place) the estimated normal of a point towards a given viewpoint
          * \param pnt a given point
          * \param vp_x the X coordinate of the viewpoint
          * \param vp_y the X coordinate of the viewpoint
          * \param vp_z the X coordinate of the viewpoint
          * \param nx the resultant X component of the plane normal
          * \param ny the resultant Y component of the plane normal
          * \param nz the resultant Z component of the plane normal
          */
        void flipNormal(const cv::Point3f &pnt, float vp_x, float vp_y, float vp_z, float &nx, float &ny, float &nz)
        {
            // See if we need to flip any plane normals
            vp_x -= pnt.x;
            vp_y -= pnt.y;
            vp_z -= pnt.z;

            // Dot product between the (viewpoint - point) and the plane normal
            float cos_theta = vp_x * nx + vp_y * ny + vp_z * nz;

            // Flip the plane normal
            if(cos_theta < 0)
            {
                nx *= -1;
                ny *= -1;
                nz *= -1;
            }
        }
};

#define callFunct(object,pointer) ((object).*(pointer))

template <typename T, int N>
struct Function {
    typedef T (IntegralImage2D<float, N>::*FctPoint)(unsigned,unsigned,unsigned,unsigned);
};

template <typename T, int N>
void sumArea(int beg_x, int beg_y, int end_x, int end_y, const int wdt, const int hgt,
              typename Function<T,N>::FctPoint f, IntegralImage2D<float,N> *img, T &rst)
{
    if(beg_x < 0)
    {
        if(beg_y < 0)
        {
            rst += callFunct(*img,f)(0, 0, end_x, end_y);
            rst += callFunct(*img,f)(0, 0, -beg_x, -beg_y);
            rst += callFunct(*img,f)(0, 0, -beg_x, end_y);
            rst += callFunct(*img,f)(0, 0, end_x, -beg_y);
        }
        else if(end_y >= hgt)
        {
            rst += callFunct(*img,f)(0, beg_y, end_x, hgt-1);
            rst += callFunct(*img,f)(0, beg_y, -beg_x, hgt-1);
            rst += callFunct(*img,f)(0, hgt-(end_y-(hgt-1)), end_x, hgt-1);
            rst += callFunct(*img,f)(0, hgt-(end_y-(hgt-1)), -beg_x, hgt-1);
        }
        else
        {
            rst += callFunct(*img,f)(0, beg_y, end_x, end_y);
            rst += callFunct(*img,f)(0, beg_y, -beg_x, end_y);
        }
    }
    else if(beg_y < 0)
    {
        if(end_x >= wdt)
        {
            rst += callFunct(*img,f)(beg_x, 0, wdt-1, end_y);
            rst += callFunct(*img,f)(beg_x, 0, wdt-1, -beg_y);
            rst += callFunct(*img,f)(wdt-(end_x-(wdt-1)), 0, wdt-1, end_y);
            rst += callFunct(*img,f)(wdt-(end_x-(wdt-1)), 0, wdt-1, -beg_y);
        }
        else
        {
            rst += callFunct(*img,f)(beg_x, 0, end_x, end_y);
            rst += callFunct(*img,f)(beg_x, 0, end_x, -beg_y);
        }
    }
    else if(end_x >= wdt)
    {
        if(end_y >= hgt)
        {
            rst += callFunct(*img,f)(beg_x, beg_y, wdt-1, hgt-1);
            rst += callFunct(*img,f)(beg_x, hgt-(end_y-(hgt-1)), wdt-1, hgt-1);
            rst += callFunct(*img,f)(wdt-(end_x-(wdt-1)), beg_y, wdt-1, hgt-1);
            rst += callFunct(*img,f)(wdt-(end_x-(wdt-1)), hgt-(end_y-(hgt-1)), wdt-1, hgt-1);
        }
        else
        {
            rst += callFunct(*img,f)(beg_x, beg_y, wdt-1, end_y);
            rst += callFunct(*img,f)(wdt-(end_x-(wdt-1)), beg_y, wdt-1, end_y);
        }
    }
    else if(end_y >= hgt)
    {
        rst += callFunct(*img,f)(beg_x, beg_y, end_x, hgt-1);
        rst += callFunct(*img,f)(beg_x, hgt-(end_y-(hgt-1)), end_x, hgt-1);
    }
    else
    {
        rst += callFunct(*img,f)(beg_x, beg_y, end_x, end_y);
    }
    //return float(callFunct(*img,f)(start_x, start_y, end_x, end_y));
}
}

#endif // NORMALESTIMATOR_H
