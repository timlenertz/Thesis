#ifndef INTEGRALIMAGE2D_H
#define INTEGRALIMAGE2D_H

#include <vector>
#include "opencv2/core/core.hpp"

#ifndef isNan
  #define isNan(a)(!(a == a))
#endif

template<typename T> bool isFinite(T arg)
{
    return arg == arg &&
           arg != std::numeric_limits<T>::infinity() &&
           arg != -std::numeric_limits<T>::infinity();
}

namespace ps {
template <typename DataType> struct TypeTraits
{
    typedef DataType Type;
    typedef DataType IntegralType;
};
template <> struct TypeTraits<float>
{
    typedef float Type;
    typedef double IntegralType;
};
template <> struct TypeTraits<char>
{
    typedef char Type;
    typedef int IntegralType;
};
template <> struct TypeTraits<short>
{
    typedef short Type;
    typedef long IntegralType;
};
template <> struct TypeTraits<unsigned short>
{
    typedef unsigned short Type;
    typedef unsigned long IntegralType;
};
template <> struct TypeTraits<unsigned char>
{
    typedef unsigned char Type;
    typedef unsigned int IntegralType;
};
template <> struct TypeTraits<int>
{
    typedef int Type;
    typedef long IntegralType;
};
template <> struct TypeTraits<unsigned int>
{
    typedef unsigned int Type;
    typedef unsigned long IntegralType;
};

/** \brief Determines an integral image representation for a given organized data array
  * \author Suat Gedikli
  */

template <class DataType, unsigned Dimension> class IntegralImage2D
{
    public:
        static const unsigned dim_fst = Dimension;
        static const unsigned dim_snd = (Dimension * (Dimension + 1)) >> 1;
        typedef cv::Vec<typename TypeTraits<DataType>::IntegralType, dim_fst> FirstType;
        typedef cv::Vec<typename TypeTraits<DataType>::IntegralType, dim_snd> SecondType;


    private:
        typedef cv::Vec<typename TypeTraits<DataType>::Type, dim_fst> InputType;

        std::vector<FirstType>  img_fst;
        std::vector<SecondType> img_snd;
        std::vector<unsigned>   img_fin;

        /** \brief The height of the 2d input data array */
        unsigned hgt;
        /** \brief The width of the 2d input data array */
        unsigned wdt;

        /** \brief Indicates whether second order integral images are available *  */
        bool com_snd;


    public:
        /** \brief Constructor for an Integral Image
          * \param[in] snd set to true if we want to compute a second order image
          */
        IntegralImage2D(bool snd = false) : img_fin(), img_fst(), img_snd(), hgt(1), wdt(1), com_snd(snd) { }

        /** \brief Destructor */
        virtual ~IntegralImage2D() { }

        /** \brief Set the input data to compute the integral image for
          * \param[in] dat the input data
          * \param[in] wdt the width of the data
          * \param[in] hgt the height of the data
          * \param[in] srd_ele the element stride of the data
          * \param[in] srd_row the row stride of the data
          */
        void setInput(const DataType* dat, unsigned wdt, unsigned hgt, unsigned srd_ele, unsigned srd_row)
        {
            if((wdt + 1) * (hgt + 1) > img_fst.size())
            {
                this->hgt = hgt;
                this->wdt = wdt;

                img_fst.resize((wdt + 1) * (hgt + 1));
                img_fin.resize((wdt + 1) * (hgt + 1));
                if(com_snd)
                    img_snd.resize((wdt + 1) * (hgt + 1));
            }
            computeIntegralImages(dat, srd_ele, srd_row);
        }
        /** \brief sets the computation for second order integral images on or off.
          * \param compute_second_order_integral_images
          */
        void setSecondOrderComputation(bool in_com_snd) { com_snd = in_com_snd; }


        /** \brief Compute the first order sum within a given rectangle
          * \param[in] start_x x position of rectangle
          * \param[in] start_y y position of rectangle
          * \param[in] width width of rectangle
          * \param[in] height height of rectangle
          */
        inline FirstType getFirstOrderSum(unsigned start_x, unsigned start_y, unsigned width, unsigned height) const
        {
            const unsigned upper_left_idx  = start_y * (wdt + 1) + start_x;
            const unsigned upper_right_idx = upper_left_idx + width;
            const unsigned lower_left_idx  =(start_y + height) * (wdt + 1) + start_x;
            const unsigned lower_right_idx = lower_left_idx + width;

            return(img_fst[lower_right_idx] + img_fst[upper_left_idx] - img_fst[upper_right_idx] - img_fst[lower_left_idx]);
        }
        /** \brief Compute the second order sum within a given rectangle
          * \param[in] start_x x position of rectangle
          * \param[in] start_y y position of rectangle
          * \param[in] width width of rectangle
          * \param[in] height height of rectangle
          */
        inline SecondType getSecondOrderSum(unsigned start_x, unsigned start_y, unsigned width, unsigned height) const
        {
            const unsigned upper_left_idx  = start_y * (wdt + 1) + start_x;
            const unsigned upper_right_idx = upper_left_idx + width;
            const unsigned lower_left_idx  = (start_y + height) * (wdt + 1) + start_x;
            const unsigned lower_right_idx = lower_left_idx + width;

            return(img_snd[lower_right_idx] + img_snd[upper_left_idx] - img_snd[upper_right_idx] - img_snd[lower_left_idx]);
        }
        /** \brief Compute the number of finite elements within a given rectangle
          * \param[in] start_x x position of rectangle
          * \param[in] start_y y position of rectangle
          * \param[in] width width of rectangle
          * \param[in] height height of rectangle
          */
        inline unsigned getFiniteElementsCount(unsigned start_x, unsigned start_y, unsigned width, unsigned height) const
        {
            const unsigned upper_left_idx  = start_y * (wdt + 1) + start_x;
            const unsigned upper_right_idx = upper_left_idx + width;
            const unsigned lower_left_idx  = (start_y + height) * (wdt + 1) + start_x;
            const unsigned lower_right_idx = lower_left_idx + width;

            return(img_fin[lower_right_idx] + img_fin[upper_left_idx] - img_fin[upper_right_idx] - img_fin[lower_left_idx]);
        }

        /** \brief Compute the first order sum within a given rectangle
          * \param[in] start_x x position of the start of the rectangle
          * \param[in] start_y x position of the start of the rectangle
          * \param[in] end_x x position of the end of the rectangle
          * \param[in] end_y x position of the end of the rectangle
          */
        inline FirstType getFirstOrderSumSE(unsigned start_x, unsigned start_y, unsigned end_x, unsigned end_y)
        {
            const unsigned upper_left_idx  = start_y * (wdt + 1) + start_x;
            const unsigned upper_right_idx = start_y * (wdt + 1) + end_x;
            const unsigned lower_left_idx  = end_y * (wdt + 1) + start_x;
            const unsigned lower_right_idx = end_y * (wdt + 1) + end_x;

            return(img_fst[lower_right_idx] + img_fst[upper_left_idx] - img_fst[upper_right_idx] - img_fst[lower_left_idx]);
        }
        /** \brief Compute the second order sum within a given rectangle
          * \param[in] start_x x position of the start of the rectangle
          * \param[in] start_y x position of the start of the rectangle
          * \param[in] end_x x position of the end of the rectangle
          * \param[in] end_y x position of the end of the rectangle
          */
        inline SecondType getSecondOrderSumSE(unsigned start_x, unsigned start_y, unsigned end_x, unsigned end_y)
        {
            const unsigned upper_left_idx  = start_y * (wdt + 1) + start_x;
            const unsigned upper_right_idx = start_y * (wdt + 1) + end_x;
            const unsigned lower_left_idx  = end_y * (wdt + 1) + start_x;
            const unsigned lower_right_idx = end_y * (wdt + 1) + end_x;

            return(img_snd[lower_right_idx] + img_snd[upper_left_idx] - img_snd[upper_right_idx] - img_snd[lower_left_idx]);
        }
        /** \brief Compute the number of finite elements within a given rectangle
          * \param[in] start_x x position of the start of the rectangle
          * \param[in] start_y x position of the start of the rectangle
          * \param[in] end_x x position of the end of the rectangle
          * \param[in] end_y x position of the end of the rectangle
          */
        inline unsigned getFiniteElementsCountSE(unsigned start_x, unsigned start_y, unsigned end_x, unsigned end_y)
        {
            const unsigned upper_left_idx  = start_y * (wdt + 1) + start_x;
            const unsigned upper_right_idx = start_y * (wdt + 1) + end_x;
            const unsigned lower_left_idx  = end_y * (wdt + 1) + start_x;
            const unsigned lower_right_idx = end_y * (wdt + 1) + end_x;

            return(img_fin[lower_right_idx] + img_fin[upper_left_idx] - img_fin[upper_right_idx] - img_fin[lower_left_idx]);
        }


    private:
        /** \brief Compute the actual integral image data
          * \param[in] dat the input data
          * \param[in] srd_ele the element stride of the data
          * \param[in] srd_row the row stride of the data
          */
        void computeIntegralImages(const DataType* dat, unsigned srd_ele, unsigned srd_row)
        {
            FirstType* previous_row = &img_fst[0];
            FirstType* current_row  = previous_row +(wdt + 1);
            memset(previous_row, 0, sizeof(FirstType) * (wdt + 1));

            unsigned* count_previous_row = &img_fin[0];
            unsigned* count_current_row  = count_previous_row + (wdt + 1);
            memset(count_previous_row, 0, sizeof(unsigned) * (wdt + 1));

            if(!com_snd)
            {
                for(unsigned row_idx = 0 ; row_idx < hgt ; row_idx++)
                {
                    current_row[0] = current_row[0].all(0);
                    count_current_row[0] = 0;

                    for(unsigned col_idx = 0, val_idx = 0 ; col_idx < wdt ; col_idx++)
                    {
                        current_row[col_idx + 1] = previous_row[col_idx + 1] + current_row[col_idx] - previous_row[col_idx];
                        count_current_row[col_idx + 1] = count_previous_row[col_idx + 1] + count_current_row[col_idx] - count_previous_row[col_idx];

                        const InputType* element = reinterpret_cast<const InputType*>(&dat[val_idx]);
                        if(isFinite(cv::sum(*element)[0]))
                        {
                            current_row[col_idx + 1] += static_cast<FirstType>(*element);
                            ++(count_current_row[col_idx + 1]);
                        }

                        //TESTER
                        //float dat[12]
                        //const Vec3f* elem = reinterpret_cast<const Vec3f*>(&dat[4]);
                        //cout << "Elem: " << elem->val[0] << ", " << elem->val[1] << ", " << elem->val[2] << endl;
                        //float scal = cv::sum(*elem)[0]; cout << "Sum: " << scal << endl;
                        //Vec3d tot(2.5, 3.0, 3.5);
                        //tot += static_cast<Vec3d>(*elem);

                        val_idx += srd_ele;
                    }
                    dat += srd_row;
                    previous_row = current_row;
                    current_row += (wdt + 1);
                    count_previous_row = count_current_row;
                    count_current_row += (wdt + 1);
                }
            }
            else
            {
                SecondType* so_previous_row = &img_snd[0];
                SecondType* so_current_row  = so_previous_row + (wdt + 1);
                memset(so_previous_row, 0, sizeof(SecondType) * (wdt + 1));

                for(unsigned row_idx = 0 ; row_idx < hgt ; row_idx++)
                {
                    current_row[0] = current_row[0].all(0);
                    so_current_row[0] = so_current_row[0].all(0);
                    count_current_row[0] = 0;
                    for(unsigned col_idx = 0, val_idx = 0 ; col_idx < wdt ; col_idx++)
                    {
                        current_row[col_idx + 1] = previous_row[col_idx + 1] + current_row[col_idx] - previous_row[col_idx];
                        so_current_row[col_idx + 1] = so_previous_row[col_idx + 1] + so_current_row[col_idx] - so_previous_row[col_idx];
                        count_current_row[col_idx + 1] = count_previous_row[col_idx + 1] + count_current_row[col_idx] - count_previous_row[col_idx];

                        const InputType* element = reinterpret_cast<const InputType*>(&dat[val_idx]);
                        if(isFinite(cv::sum(*element)[0]))
                        {
                            current_row[col_idx + 1] += static_cast<FirstType>(*element);
                            ++(count_current_row[col_idx + 1]);

                            for(unsigned my_idx = 0, el_idx = 0; my_idx < Dimension; my_idx++)
                                for(unsigned mx_idx = my_idx; mx_idx < Dimension; mx_idx++, el_idx++)
                                    so_current_row[col_idx + 1][el_idx] += (*element)[my_idx] * (*element)[mx_idx];
                        }
                        val_idx += srd_ele;
                    }

                    dat += srd_row;
                    previous_row = current_row;
                    current_row += (wdt + 1);
                    count_previous_row = count_current_row;
                    count_current_row += (wdt + 1);
                    so_previous_row = so_current_row;
                    so_current_row += (wdt + 1);
                }
            }
        }
};

/**
* \brief partial template specialization for integral images with just one channel.
  */
template <class DataType> class IntegralImage2D <DataType, 1>
{
    public:
        static const unsigned dim_fst = 1;
        static const unsigned dim_snd = 1;
        typedef typename TypeTraits<DataType>::IntegralType FirstType;
        typedef typename TypeTraits<DataType>::IntegralType SecondType;


    private:
        std::vector<FirstType>  img_fst;
        std::vector<SecondType> img_snd;
        std::vector<unsigned>   img_fin;

        /** \brief The height of the 2d input data array */
        unsigned hgt;
        /** \brief The width of the 2d input data array */
        unsigned wdt;

        /** \brief Indicates whether second order integral images are available *  */
        bool com_snd;


    public:
        /** \brief Constructor for an Integral Image
          * \param[in] snd set to true if we want to compute a second order image
          */
        IntegralImage2D(bool snd = false) : img_fin(), img_fst(), img_snd(), hgt(1), wdt(1), com_snd(snd) { }

        /** \brief Destructor */
        virtual ~IntegralImage2D() { }

        /** \brief Set the input data to compute the integral image for
          * \param[in] dat the input data
          * \param[in] wdt the width of the data
          * \param[in] hgt the height of the data
          * \param[in] srd_ele the element stride of the data
          * \param[in] srd_row the row stride of the data
          */
        void setInput(const DataType* dat, unsigned wdt, unsigned hgt, unsigned srd_ele, unsigned srd_row)
        {
            if((wdt + 1) * (hgt + 1) > img_fst.size())
            {
                this->hgt = hgt;
                this->wdt = wdt;

                img_fst.resize((wdt + 1) * (hgt + 1));
                img_fin.resize((wdt + 1) * (hgt + 1));
                if(com_snd)
                    img_snd.resize((wdt + 1) * (hgt + 1));
            }
            computeIntegralImages(dat, srd_ele, srd_row);
        }
        /** \brief sets the computation for second order integral images on or off.
          * \param compute_second_order_integral_images
          */
        void setSecondOrderComputation(bool in_com_snd) { com_snd = in_com_snd; }


        /** \brief Compute the first order sum within a given rectangle
          * \param[in] start_x x position of rectangle
          * \param[in] start_y y position of rectangle
          * \param[in] width width of rectangle
          * \param[in] height height of rectangle
          */
        inline FirstType getFirstOrderSum(unsigned start_x, unsigned start_y, unsigned width, unsigned height) const
        {
            const unsigned upper_left_idx  = start_y * (wdt + 1) + start_x;
            const unsigned upper_right_idx = upper_left_idx + width;
            const unsigned lower_left_idx  = (start_y + height) * (wdt + 1) + start_x;
            const unsigned lower_right_idx = lower_left_idx + width;

            return(img_fst[lower_right_idx] + img_fst[upper_left_idx] - img_fst[upper_right_idx] - img_fst[lower_left_idx]);
        }
        /** \brief Compute the second order sum within a given rectangle
          * \param[in] start_x x position of rectangle
          * \param[in] start_y y position of rectangle
          * \param[in] width width of rectangle
          * \param[in] height height of rectangle
          */
        inline SecondType getSecondOrderSum(unsigned start_x, unsigned start_y, unsigned width, unsigned height) const
        {
            const unsigned upper_left_idx  = start_y * (wdt + 1) + start_x;
            const unsigned upper_right_idx = upper_left_idx + width;
            const unsigned lower_left_idx  = (start_y + height) * (wdt + 1) + start_x;
            const unsigned lower_right_idx = lower_left_idx + width;

            return(img_snd[lower_right_idx] + img_snd[upper_left_idx] - img_snd[upper_right_idx] - img_snd[lower_left_idx]);
        }
        /** \brief Compute the number of finite elements within a given rectangle
          * \param[in] start_x x position of rectangle
          * \param[in] start_y y position of rectangle
          * \param[in] width width of rectangle
          * \param[in] height height of rectangle
          */
        inline unsigned getFiniteElementsCount(unsigned start_x, unsigned start_y, unsigned width, unsigned height) const
        {
            const unsigned upper_left_idx  = start_y * (wdt + 1) + start_x;
            const unsigned upper_right_idx = upper_left_idx + width;
            const unsigned lower_left_idx  = (start_y + height) * (wdt + 1) + start_x;
            const unsigned lower_right_idx = lower_left_idx + width;

            return(img_fin[lower_right_idx] + img_fin[upper_left_idx] - img_fin[upper_right_idx] - img_fin[lower_left_idx]);
        }

        /** \brief Compute the first order sum within a given rectangle
          * \param[in] start_x x position of the start of the rectangle
          * \param[in] start_y x position of the start of the rectangle
          * \param[in] end_x x position of the end of the rectangle
          * \param[in] end_y x position of the end of the rectangle
          */
        inline FirstType getFirstOrderSumSE(unsigned start_x, unsigned start_y, unsigned end_x, unsigned end_y)
        {
            const unsigned upper_left_idx  = start_y * (wdt + 1) + start_x;
            const unsigned upper_right_idx = start_y * (wdt + 1) + end_x;
            const unsigned lower_left_idx  = end_y * (wdt + 1) + start_x;
            const unsigned lower_right_idx = end_y * (wdt + 1) + end_x;

            return(img_fst[lower_right_idx] + img_fst[upper_left_idx] - img_fst[upper_right_idx] - img_fst[lower_left_idx]);
        }
        /** \brief Compute the second order sum within a given rectangle
          * \param[in] start_x x position of the start of the rectangle
          * \param[in] start_y x position of the start of the rectangle
          * \param[in] end_x x position of the end of the rectangle
          * \param[in] end_y x position of the end of the rectangle
          */
        inline SecondType getSecondOrderSumSE(unsigned start_x, unsigned start_y, unsigned end_x, unsigned end_y)
        {
            const unsigned upper_left_idx  = start_y * (wdt + 1) + start_x;
            const unsigned upper_right_idx = start_y * (wdt + 1) + end_x;
            const unsigned lower_left_idx  = end_y * (wdt + 1) + start_x;
            const unsigned lower_right_idx = end_y * (wdt + 1) + end_x;

            return(img_snd[lower_right_idx] + img_snd[upper_left_idx] - img_snd[upper_right_idx] - img_snd[lower_left_idx]);
        }
        /** \brief Compute the number of finite elements within a given rectangle
          * \param[in] start_x x position of the start of the rectangle
          * \param[in] start_y x position of the start of the rectangle
          * \param[in] end_x x position of the end of the rectangle
          * \param[in] end_y x position of the end of the rectangle
          */
        inline unsigned getFiniteElementsCountSE(unsigned start_x, unsigned start_y, unsigned end_x, unsigned end_y)
        {
            const unsigned upper_left_idx  = start_y * (wdt + 1) + start_x;
            const unsigned upper_right_idx = start_y * (wdt + 1) + end_x;
            const unsigned lower_left_idx  = end_y * (wdt + 1) + start_x;
            const unsigned lower_right_idx = end_y * (wdt + 1) + end_x;

            return(img_fin[lower_right_idx] + img_fin[upper_left_idx] - img_fin[upper_right_idx] - img_fin[lower_left_idx]);
        }


    private:
        /** \brief Compute the actual integral image data
          * \param[in] dat the input data
          * \param[in] srd_ele the element stride of the data
          * \param[in] srd_row the row stride of the data
          */
        void computeIntegralImages(const DataType* dat, unsigned srd_ele, unsigned srd_row)
        {
            FirstType* previous_row = &img_fst[0];
            FirstType* current_row  = previous_row +(wdt + 1);
            memset(previous_row, 0, sizeof(FirstType) *(wdt + 1));

            unsigned* count_previous_row = &img_fin[0];
            unsigned* count_current_row  = count_previous_row + (wdt + 1);
            memset(count_previous_row, 0, sizeof(unsigned) * (wdt + 1));

            if(!com_snd)
            {
                for(unsigned row_idx = 0 ; row_idx < hgt ; row_idx++)
                {
                    current_row[0] = 0.0;
                    count_current_row[0] = 0;

                    for(unsigned col_idx = 0, val_idx = 0 ; col_idx < wdt ; col_idx++)
                    {
                        current_row[col_idx + 1] = previous_row[col_idx + 1] + current_row[col_idx] - previous_row[col_idx];
                        count_current_row[col_idx + 1] = count_previous_row[col_idx + 1] + count_current_row[col_idx] - count_previous_row[col_idx];

                        if(isFinite(dat[val_idx]))
                        {
                            current_row[col_idx + 1] += dat[val_idx];
                            ++(count_current_row[col_idx + 1]);
                        }
                        val_idx += srd_ele;
                    }
                    dat += srd_row;
                    previous_row = current_row;
                    current_row += (wdt + 1);
                    count_previous_row = count_current_row;
                    count_current_row += (wdt + 1);
                }
            }
            else
            {
                SecondType* so_previous_row = &img_snd[0];
                SecondType* so_current_row  = so_previous_row + (wdt + 1);
                memset(so_previous_row, 0, sizeof(SecondType) * (wdt + 1));

                for(unsigned row_idx = 0; row_idx < hgt ; row_idx++)
                {
                    current_row[0] = 0.0;
                    so_current_row[0] = 0.0;
                    count_current_row[0] = 0;

                    for(unsigned col_idx = 0, val_idx = 0 ; col_idx < wdt ; col_idx++)
                    {
                        current_row[col_idx + 1] = previous_row[col_idx + 1] + current_row[col_idx] - previous_row[col_idx];
                        so_current_row[col_idx + 1] = so_previous_row[col_idx + 1] + so_current_row[col_idx] - so_previous_row[col_idx];
                        count_current_row[col_idx + 1] = count_previous_row[col_idx + 1] + count_current_row[col_idx] - count_previous_row[col_idx];

                        if(isFinite(dat[val_idx]))
                        {
                            current_row[col_idx + 1] += dat[val_idx];
                            so_current_row[col_idx + 1] += dat[val_idx] * dat[val_idx];
                            ++(count_current_row[col_idx + 1]);
                        }
                        val_idx += srd_ele;
                    }
                    dat += srd_row;
                    previous_row = current_row;
                    current_row += (wdt + 1);
                    count_previous_row = count_current_row;
                    count_current_row += (wdt + 1);
                    so_previous_row = so_current_row;
                    so_current_row += (wdt + 1);
                }
            }
        }
};


//template<typename T> bool isNan2(const T& arg)
//{
//    return !(arg == arg);
//}
}

#endif // INTEGRALIMAGE2D_H
