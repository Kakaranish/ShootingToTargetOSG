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
    std::vector<Ball *> balls;
    osg::ref_ptr<osg::MatrixTransform> createCannon(osg::Vec3f defaultPosition = osg::Vec3f(0, -20, 0));

public:
    const float WheelRadius;
    const float WheelThickness;
    const float RotationAngle;
    const float BarrelRadius;
    const float BarrelWidth;
    const float BarrelDefaultAngle;
    float BarrelRotationAngle;

    osg::Vec3f barrelAnchor;
    osg::ref_ptr<osg::Cylinder> barrelCylinder;
    osg::ref_ptr<osg::Geode> barrelGeode;
    osg::ref_ptr<osg::MatrixTransform> barrelMatrix;
    osg::ref_ptr<osg::Group> _world;

    Cannon(osg::ref_ptr<osg::Group> world, osg::Vec3f position = osg::Vec3f(0, -20, 0))
        : WheelRadius(0.8f),
          WheelThickness(0.45f),
          RotationAngle(osg::PI_2),
          BarrelRadius(0.3f),
          BarrelWidth(5.f),
          BarrelDefaultAngle(degreesToRadians(-45))
    {
        BarrelRotationAngle = BarrelDefaultAngle;
        _world = world;
        _cannonMatrix = createCannon();
    }

    void move(osg::Vec3f movement)
    {
        osg::Vec3f newPosition = _cannonMatrix->getMatrix().getTrans() + movement;
        // barrelAnchor += movement;
        
        _cannonMatrix->setMatrix(osg::Matrix::translate(newPosition));
    }

    void setRotation(float degrees)
    {
        BarrelRotationAngle = degreesToRadians(degrees);
        barrelMatrix->setMatrix(
            osg::Matrix::rotate(osg::Quat(BarrelRotationAngle, osg::X_AXIS)) *
            osg::Matrix::translate(barrelAnchor));
        barrelCylinder->setRotation(osg::Quat(BarrelRotationAngle, osg::X_AXIS));
    }

    void rotate(float degrees)
    {
        BarrelRotationAngle += degreesToRadians(degrees);
        barrelMatrix->setMatrix(
            osg::Matrix::rotate(osg::Quat(BarrelRotationAngle, osg::X_AXIS)) *
            osg::Matrix::translate(barrelAnchor));
    }

    Ball *createBall()
    {
        float zOffset = BarrelWidth / 2 * sin((osg::PI_2 - abs(BarrelRotationAngle)));
        float yOffset = BarrelWidth / 2 * cos((osg::PI_2 - abs(BarrelRotationAngle)));
        osg::Vec3f ballPositionOffset(0, yOffset, zOffset);
        return new Ball(_world, barrelAnchor + ballPositionOffset);
    }

    osg::ref_ptr<osg::MatrixTransform> getMatrixTransform();
};

#endif