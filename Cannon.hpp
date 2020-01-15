#ifndef CANNON_HPP
#define CANNON_HPP

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include "Utility.hpp"
#include "Ball.hpp"
#include <math.h>

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
    float BarrelRotationAngle;

    osg::Vec3f barrelCenter;
    osg::Vec3f barrelAnchor;
    osg::ref_ptr<osg::Cylinder> barrelCylinder;

    Cannon(float angle, osg::Vec3f position = osg::Vec3f(0, -20, 0))
        : WheelRadius(0.8f),
          WheelThickness(0.45f),
          RotationAngle(osg::PI_2),
          BarrelRadius(0.3f),
          BarrelWidth(5.f),
          BarrelRotationAngle(degreesToRadians(angle))
    {
        _cannonMatrix = createCannon();
    }

    void setRotation(float degrees)
    {
        BarrelRotationAngle = degreesToRadians(degrees);
        barrelCylinder->setRotation(osg::Quat(BarrelRotationAngle, osg::X_AXIS));
    }

    void rotate(float degrees)
    {
        BarrelRotationAngle += degreesToRadians(degrees);
        barrelCylinder->setRotation(osg::Quat(BarrelRotationAngle, osg::X_AXIS));
    }

    Ball *getBall()
    {
        float zOffset = BarrelWidth / 2 * sin((osg::PI_2 - abs(BarrelRotationAngle)));
        float yOffset = BarrelWidth / 2 * cos((osg::PI_2 - abs(BarrelRotationAngle)));
        osg::Vec3f ballPositionOffset(0, yOffset, zOffset);
        return new Ball(barrelAnchor + ballPositionOffset);
    }

    osg::ref_ptr<osg::MatrixTransform> getMatrixTransform();
};

#endif