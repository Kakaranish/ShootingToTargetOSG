#ifndef CANNON_HPP
#define CANNON_HPP

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include "Utility.hpp"

class Cannon
{
private:
    osg::ref_ptr<osg::MatrixTransform> _cannonMatrix;

    osg::ref_ptr<osg::MatrixTransform> createCannon(osg::Vec3f defaultPosition = osg::Vec3f(0, -20, 0));

public:
    const float WheelRadius;
    const float WheelThickness;
    const float RotationAngle;
    const float BarrelRadius;
    const float BarrelWidth;
    const float BarrelRotationAngle;

    Cannon(osg::Vec3f position = osg::Vec3f(0, -20, 0))
        : WheelRadius(0.8f),
          WheelThickness(0.45f),
          RotationAngle(osg::PI_2),
          BarrelRadius(0.3f),
          BarrelWidth(5.f),
          BarrelRotationAngle(-0.8 * osg::PI_2)
    {
        _cannonMatrix = createCannon();
    }

    osg::ref_ptr<osg::MatrixTransform> getMatrixTransform();
};

#endif