#ifndef ENUMS_H
#define ENUMS_H

#define PS_PI       3.14159265358979323846
#define PS_PI_2     1.57079632679489661923
#define PS_1_PI2    0.1013211836423378

namespace ps {

    /** \brief Different types of border handling. */
    enum BorderMethod
    {
        BORDER_POLICY_IGNORE = 0,
        BORDER_POLICY_MIRROR = 1
    };
    /** \brief Different normal estimation methods.
      *   COVARIANCE_MATRIX - creates 9 integral images to compute the normal for a specific point
      *   from the covariance matrix of its local neighborhood.
      *   AVERAGE_3D_GRADIENT - creates 6 integral images to compute smoothed versions of
      *   horizontal and vertical 3D gradients and computes the normals using the cross-product between these
      *   two gradients.
      *   AVERAGE_DEPTH_CHANGE -  creates only a single integral image and computes the normals
      *   from the average depth changes.
      */
    enum NormalMethod
    {
        AVERAGE_3D_GRADIENT  = 0,
        AVERAGE_DEPTH_CHANGE = 1,
        COVARIANCE_MATRIX    = 2,
        SIMPLE_3D_GRADIENT   = 3
    };
    /** \brief Different types of coordinate system handling. */
    enum CoordinateMethod
    {
        SYSTEM_CARTESIAN     = 0,
        SYSTEM_CYLINDRICAL   = 1
    };
}
#endif // ENUMS_H
