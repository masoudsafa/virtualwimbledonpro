/*==============================================================================
            Copyright (c) 2010-2011 QUALCOMM Incorporated.
            All Rights Reserved.
            Qualcomm Confidential and Proprietary
			
@file 
    Tool.h

@brief
    Header file for global Tool functions.

==============================================================================*/
#ifndef _QCAR_TOOL_H_
#define _QCAR_TOOL_H_

// Include files
#include <QCAR/System.h>
#include <QCAR/Matrices.h>
#include <QCAR/Vectors.h>

namespace QCAR
{

// Forward declarations
class CameraCalibration;

/// Tool functions
namespace Tool
{
    /// Returns a 4x4 col-major OpenGL model-view matrix from a 3x4 QCAR pose
    /// matrix.
    /**
     *  QCAR uses 3x4 row-major matrices for pose data. convertPose2GLMatrix()
     *  takes such a pose matrix and returns an OpenGL compatible model-view
     *  matrix.
     */
    QCAR_API Matrix44F convertPose2GLMatrix(const Matrix34F& pose);

    /// Returns an OpenGL style projection matrix.
    QCAR_API Matrix44F getProjectionGL(const CameraCalibration& calib,
                                        float nearPlane, float farPlane);

    /// Projects a 3D scene point onto the screen (device coordinates) given
    /// a pose in form of a 3x4 matrix.
    /**
     *  projectPoint() takes a 3D point in scene coordinates and transforms it
     *  using the given pose matrix. It then projects it onto the screen (pixel
     *  coordinates) using the given camera calibration.
     */
    QCAR_API Vec2F projectPoint(const CameraCalibration& calib,
                                    const Matrix34F& pose, const Vec3F& point);

    /// Multiplies two QCAR pose matrices
    /**
     *  In order to apply a transformation A on top of a transformation B,
     *  perform: multiply(B,A).
     */
    QCAR_API Matrix34F multiply(const Matrix34F& matLeft,
                                const Matrix34F& matRight);

    /// Multiplies two QCAR-style 4x4-matrices (row-major order)
    QCAR_API Matrix44F multiply(const Matrix44F& matLeft,
                                const Matrix44F& matRight);

    /// Multiplies two GL-style matrices (col-major order)
    QCAR_API Matrix44F multiplyGL(const Matrix44F& matLeft,
                                  const Matrix44F& matRight);

    /// Sets the translation part of a 3x4 pose matrix
    QCAR_API void setTranslation(Matrix34F& pose,
                                 const Vec3F& translation);

    /// Sets the rotation part of a 3x4 pose matrix using axis-angle as input
    /**
     *  The axis parameter defines the 3D axis around which the pose rotates.
     *  The angle parameter defines the angle in degrees for the rotation
     *  around that axis.
     */
    QCAR_API void setRotation(Matrix34F& pose,
                              const Vec3F& axis, float angle);

} // namespace Tool

} // namespace QCAR

#endif //_QCAR_TOOL_H_
