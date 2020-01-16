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
    float xOffset;

    osg::Vec3f barrelAnchor;
    osg::ref_ptr<osg::Cylinder> barrelCylinder;
    osg::ref_ptr<osg::Geode> barrelGeode;
    osg::ref_ptr<osg::MatrixTransform> barrelMatrix;
    osg::ref_ptr<osg::Group> _world;
    osg::Vec3f _position;

    Cannon(osg::ref_ptr<osg::Group> world, osg::Vec3f position = osg::Vec3f(0, -20, 0))
        : WheelRadius(0.8f),
          WheelThickness(0.45f),
          RotationAngle(osg::PI_2),
          BarrelRadius(0.3f),
          BarrelWidth(5.f),
          BarrelDefaultAngle(degreesToRadians(-45))
    {
        // xPosition = position.x();
        _position = position;
        BarrelRotationAngle = BarrelDefaultAngle;
        _world = world;
        _cannonMatrix = createCannon(_position);
    }

    void move(float xMovement)
    {
        osg::Vec3f newPosition = _cannonMatrix->getMatrix().getTrans() +
                                 osg::Vec3f(xMovement, 0.f, 0.f);

        _position += osg::Vec3f(xMovement, 0, 0);

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
        osg::Vec3f ballOffset(
            _position.x(),
            BarrelWidth / 2 * cos(osg::PI_2 - abs(BarrelRotationAngle)),
            BarrelWidth / 2 * sin(osg::PI_2 - abs(BarrelRotationAngle)));
        osg::Vec3f ballPosition = barrelAnchor + ballOffset;

        const float speed = 0.6;
        std::cout << cos(BarrelRotationAngle) * speed << ", " << sin(BarrelRotationAngle) * speed << std::endl;
        osg::Vec3f velocity(
            0,
            cos(osg::PI_2 - abs(BarrelRotationAngle)) * speed,
            sin(osg::PI_2 - abs(BarrelRotationAngle)) * speed);

        return new Ball(_world, ballPosition, 0.4, velocity);
    }

    osg::ref_ptr<osg::MatrixTransform> getMatrixTransform();
};

#endif